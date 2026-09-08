#include "EdGraphSchema_DialogAsset.h"
#include "AssetTypeActions_Base.h"
#include "DialogAssetEditorTypes.h"
#include "DialogAssetGraph.h"
#include "DialogAssetGraphNode.h"
#include "DialogAssetGraphNode_Choice.h"
#include "DialogAssetGraphNode_Root.h"
#include "EdGraph/EdGraphPin.h"
#include "EdGraph/EdGraphSchema.h"
#include "EdGraphNode_Comment.h"
#include "Framework/Commands/Commands.h"
#include "Framework/Commands/UIAction.h"
#include "Framework/Commands/UICommandInfo.h"
#include "GenericPlatform/GenericApplication.h"
#include "GraphEditor.h"
#include "InputCoreTypes.h"
#include "Internationalization/Text.h"
#include "Modules/ModuleManager.h"
#include "ScopedTransaction.h"
#include "AIGraphTypes.h"
#include "DialogGraph_Editor.h"
#include "Styling/AppStyle.h"
#include "Textures/SlateIcon.h"
#include "ToolMenuSection.h"
#include "UObject/UObjectGlobals.h"
#include "ToolMenu.h"
#include "Framework/Commands/GenericCommands.h"

namespace
{
    // Maximum distance a drag can be off a node edge to require 'push off' from node
    const int32 NodeDistance = 60;
}

UEdGraphNode* FDialogSchemaAction_AddComment::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode)
{
    UEdGraphNode_Comment* const CommentTemplate = NewObject<UEdGraphNode_Comment>();

    FVector2f SpawnLocation = Location;
    FSlateRect Bounds;

    TSharedPtr<SGraphEditor> GraphEditorPtr = SGraphEditor::FindGraphEditorForGraph(ParentGraph);
    if(GraphEditorPtr.IsValid())
    {
        if(GraphEditorPtr->GetBoundsForSelectedNodes(Bounds, 50.0f))
        {
            CommentTemplate->SetBounds(Bounds);
            SpawnLocation.X = CommentTemplate->NodePosX;
            SpawnLocation.Y = CommentTemplate->NodePosY;
        }
        else
        {
            SpawnLocation = GraphEditorPtr->GetPasteLocation2f();
        }
    }

    UEdGraphNode* const NewNode = FEdGraphSchemaAction_NewNode::SpawnNodeFromTemplate<UEdGraphNode_Comment>(ParentGraph, CommentTemplate, SpawnLocation, bSelectNewNode);
    return NewNode;
}

UEdGraphNode* FDialogSchemaAction_NewNode::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode)
{
    UEdGraphNode* ResultNode = NULL;

    if(NodeTemplate != NULL)
    {
        const FScopedTransaction Transaction(FText::FromString("Add Node"));
        ParentGraph->Modify();
        if(FromPin)
        {
            FromPin->Modify();
        }

        NodeTemplate->SetFlags(RF_Transactional);

        NodeTemplate->Rename(NULL, ParentGraph, REN_NonTransactional);
        ParentGraph->AddNode(NodeTemplate, true);

        NodeTemplate->CreateNewGuid();
        NodeTemplate->PostPlacedNewNode();

        // For input pins, new node will generally overlap node being dragged off
        // Work out if we want to visually push away from connected ndoe
        int32 XLocation = static_cast<int32>(Location.X);
        if(FromPin && FromPin->Direction == EGPD_Input)
        {
            UEdGraphNode* PinNode = FromPin->GetOwningNode();
            const float XDelta = FMath::Abs(PinNode->NodePosX - Location.X);

            if(XDelta < NodeDistance)
            {
                // Set location to edge of current node minus the max move distance
                // to force node to push off from connect node enough to give selection handle
                XLocation = PinNode->NodePosX - NodeDistance;
            }
        }

        NodeTemplate->NodePosX = XLocation;
        NodeTemplate->NodePosY = static_cast<int32>(Location.Y);

        // Setup pins after placing node in correct spot, since pin sorting will happen as soon as link connection change occurs
        NodeTemplate->AllocateDefaultPins();
        NodeTemplate->AutowireNewNode(FromPin);

        ResultNode = NodeTemplate;
    }

    return ResultNode;
}

void FDialogSchemaAction_NewNode::AddReferencedObjects(FReferenceCollector& Collector) 
{
    FEdGraphSchemaAction::AddReferencedObjects(Collector);

    // These don't get saved to disk, but we want to make sure the objects don't get GC'd while the action array is around
    Collector.AddReferencedObject(NodeTemplate);
}


UEdGraphSchema_DialogAsset::UEdGraphSchema_DialogAsset(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UEdGraphSchema_DialogAsset::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
    FGraphNodeCreator<UDialogAssetGraphNode_Root> NodeCreator(Graph);
    UDialogAssetGraphNode_Root* MyNode = NodeCreator.CreateNode(true, Cast<UDialogAssetGraph>(&Graph)->RootNodeClass);
    NodeCreator.Finalize();
    SetNodeMetaData(MyNode, FNodeMetadata::DefaultGraphNode);
}

void UEdGraphSchema_DialogAsset::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
    FGraphNodeClassHelper& ClassCache = GetClassCache();
    FCategorizedGraphActionListBuilder NodesBuilder(TEXT("Nodes"));

    // Add the ability to create different dialog-related nodes to the context menu
    {
        TArray<FGraphNodeClassData> NodeClasses;
        ClassCache.GatherClasses(UDialogAssetGraphNode::StaticClass(), NodeClasses);

        const FString BaseClassName = UDialogAssetGraphNode::StaticClass()->GetName();
        const FString RootClassName = UDialogAssetGraphNode_Root::StaticClass()->GetName();
        for(auto& NodeClass : NodeClasses)
        {
            if(NodeClass.GetClassName() == BaseClassName) continue;
            if(NodeClass.GetClassName() == RootClassName) continue;

            const FText NodeTypeName = FText::FromString(FName::NameToDisplayString(NodeClass.ToString(), false));

            TSharedPtr<FDialogSchemaAction_NewNode> AddOpAction = UEdGraphSchema_DialogAsset::AddNewNodeAction(NodesBuilder, NodeClass.GetCategory(), NodeTypeName, NodeClass.GetTooltip());

            UDialogAssetGraphNode* OpNode = NewObject<UDialogAssetGraphNode>(ContextMenuBuilder.OwnerOfTemporaries, NodeClass.GetClass());
            AddOpAction->NodeTemplate = OpNode;
        }
        ContextMenuBuilder.Append(NodesBuilder);
    }

    // Add the ability to create a comment to the context menu
    {
        TSharedPtr<FDialogSchemaAction_AddComment> Action = TSharedPtr<FDialogSchemaAction_AddComment>(new FDialogSchemaAction_AddComment(FText::FromString("Add Comment"), FText::FromString("Adds a comment node to the graph. ")));
        ContextMenuBuilder.AddAction(Action);
    }
}


const FPinConnectionResponse UEdGraphSchema_DialogAsset::CanCreateConnection(const UEdGraphPin* PinA, const UEdGraphPin* PinB) const
{
    if(PinA->GetOwningNode() == PinB->GetOwningNode()) return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Can't connect a Node with itself"));
    if(PinA->Direction == PinB->Direction)
    {
        if(PinA->Direction == EGPD_Input) return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Can't connect input pin to input pin"));
        if(PinA->Direction == EGPD_Output) return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Can't connect output pin to output pin"));
    }

    const bool bPinAHasConnection = PinA->HasAnyConnections();
    const bool bPinBHasConnection = PinB->HasAnyConnections();
    const bool bPinAIsSingleMode = PinA->PinType.PinCategory == UDialogAssetEditorTypes::PinCategory_SingleNode;
    const bool bPinBIsSingleMode = PinB->PinType.PinCategory == UDialogAssetEditorTypes::PinCategory_SingleNode;

    if(bPinAHasConnection && bPinAIsSingleMode)
    {
        if(bPinBHasConnection && bPinBIsSingleMode)
        {
            return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT("Replace connection"));
        }
        else
        {
            return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_A, TEXT("Replace connection"));
        }
    }
    else if(bPinBHasConnection && bPinBIsSingleMode)
    {
        if(bPinAHasConnection && bPinAIsSingleMode)
        {
            return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT("Replace connection"));
        }
        else
        {
            return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_B, TEXT("Replace connection"));
        }
    }

    return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT("Make Connection"));
}

void UEdGraphSchema_DialogAsset::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
    if(Context->Node)
    {
        FToolMenuSection& Section = Menu->AddSection("DialogAssetGraphSchemaNodeActions", FText::FromString("Node Actions"));
        Section.AddMenuEntry(FGenericCommands::Get().Delete);
        Section.AddMenuEntry(FGenericCommands::Get().Cut);
        Section.AddMenuEntry(FGenericCommands::Get().Copy);
        Section.AddMenuEntry(FGenericCommands::Get().Duplicate);
    }

    if(Context->Pin && Context->Pin->PinType.PinSubCategory == UDialogAssetEditorTypes::PinSubCategory_ChoiceNode)
    {
        FToolMenuSection& Section = Menu->AddSection("DialogAssetGraphSchemaPinActions", FText::FromString("Branch Pin Actions"));

        UDialogAssetGraphNode_Choice* ChoiceNode = Cast<UDialogAssetGraphNode_Choice>(Context->Pin->GetOwningNode());
        UEdGraphPin* PinToRemove = const_cast<UEdGraphPin*>(Context->Pin);

        Section.AddMenuEntry(
                "Remove Pin", 
                FText::FromString("Remove Pin"), 
                FText::FromString("Remove the selected pin"),
                FSlateIcon(FAppStyle::GetAppStyleSetName(), "GenericCommands.Delete"),
                FUIAction(FExecuteAction::CreateUObject(ChoiceNode, &UDialogAssetGraphNode_Choice::RemovePin, PinToRemove))
                );
    }

    Super::GetContextMenuActions(Menu, Context);
}

TSharedPtr<FDialogSchemaAction_NewNode> UEdGraphSchema_DialogAsset::AddNewNodeAction(FGraphActionListBuilderBase& ContextMenuBuilder, const FText& Category, const FText& MenuDesc, const FText& Tooltip)
{
    TSharedPtr<FDialogSchemaAction_NewNode> NewAction = TSharedPtr<FDialogSchemaAction_NewNode>(new FDialogSchemaAction_NewNode(Category, MenuDesc, Tooltip, 0));
    ContextMenuBuilder.AddAction(NewAction);

    return NewAction;
}

FGraphNodeClassHelper& UEdGraphSchema_DialogAsset::GetClassCache() const
{
    const FDialogGraph_EditorModule& EditorModule = FModuleManager::GetModuleChecked<FDialogGraph_EditorModule>("DialogGraph_Editor");
    FGraphNodeClassHelper* ClassHelper = EditorModule.GetClassCache().Get();
    check(ClassHelper);
    return *ClassHelper;
}
