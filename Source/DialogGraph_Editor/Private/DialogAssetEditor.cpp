#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphPin.h"
#include "EditorGraph/GraphNodes/DialogGraphNode_Base.h"
#include "UObject/UObjectGlobals.h"
#include <DialogAssetEditor.h>
#include <EditorGraph/DialogGraphEditMode.h>
#include <EditorGraph/DialogGraphSchema.h>
#include <DialogAsset.h>
#include <Kismet2/BlueprintEditorUtils.h>
#include <EditorGraph/GraphNodes/DialogGraphNode_Start.h>
#include <EditorGraph/GraphNodes/DialogGraphNode_Line.h>
#include <EditorGraph/GraphNodes/DialogGraphNode_Choice.h>
#include <GraphEditAction.h>
#include <DialogNode.h>

void FDialogAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& tabManager)
{
    FWorkflowCentricApplication::RegisterTabSpawners(tabManager);
}

void FDialogAssetEditor::InitEditor(const EToolkitMode::Type mode, const TSharedPtr<IToolkitHost>& initToolkitHost, UObject* inObject)
{
    _WorkingAsset = Cast<UDialogAsset>(inObject);
    _WorkingGraph = FBlueprintEditorUtils::CreateNewGraph(_WorkingAsset, *_WorkingAsset->GetName(), UEdGraph::StaticClass(), UDialogGraphSchema::StaticClass());

    TSharedPtr<FExtender> compileExtension = MakeShareable(new FExtender);

    compileExtension->AddToolBarExtension
        (
            TEXT("Asset"),
            EExtensionHook::After,
            GetToolkitCommands(),
            FToolBarExtensionDelegate::CreateSP(this, &FDialogAssetEditor::FillToolbar)
        );

    AddToolbarExtender(compileExtension);

    InitAssetEditor(mode, initToolkitHost, TEXT("DialogAssetEditor"), FTabManager::FLayout::NullLayout, true, true, inObject);

    AddApplicationMode(TEXT("DialogGraphEditMode"), MakeShareable(new FDialogGraphEditMode(SharedThis(this))));
    SetCurrentMode(TEXT("DialogGraphEditMode"));

    InitializeGraphFromAsset();
}

void FDialogAssetEditor::FillToolbar(FToolBarBuilder& Builder)
{
    Builder.AddToolBarButton
    (
        FUIAction(FExecuteAction::CreateSP(this, &FDialogAssetEditor::UpdateAssetFromGraph)),
        NAME_None,
        FText::FromString("Compile"),
        FText::FromString("Compile the dialog graph into runtime data."),
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "Kismet.Compile")
    );
}

void FDialogAssetEditor::OnClose() 
{
    //UpdateAssetFromGraph();
}

void FDialogAssetEditor::InitializeGraphFromAsset()
{
    UDialogGraphNode_Start* startNode = NewObject<UDialogGraphNode_Start>(_WorkingGraph);
    FEditorData editorData;
    startNode->SetupNode(&editorData);
    _WorkingGraph->AddNode(startNode, true, true);

    TMap<int, UDialogGraphNode_Base*> IndexToGraphNode;
    for(int i = 0; i < _WorkingAsset->GetNodeCount(); i++)
    {
        FDialogNode* runtimeNode = _WorkingAsset->GetNode(i);
        UDialogGraphNode_Base* graphNode = nullptr;

        switch(runtimeNode->NodeType)
        {
            case ENodeType::Line:
                graphNode = NewObject<UDialogGraphNode_Line>(_WorkingGraph);
                break;
            case ENodeType::Choice:
                graphNode = NewObject<UDialogGraphNode_Choice>(_WorkingGraph);
                break;
        }

        graphNode->SetPinInfo(runtimeNode->NextIDs);
        graphNode->SetupNode(&runtimeNode->editorData);

        IndexToGraphNode.Emplace(runtimeNode->ID, graphNode);
        _WorkingGraph->AddNode(graphNode, true, true);
    }

    startNode->TryConnectToNode(startNode->GetPinAt(0), IndexToGraphNode.FindRef(_WorkingAsset->GetStartNodeID())); // We connect the start node to the first node

    for(int t = 0; t < _WorkingAsset->GetNodeCount(); t++)
    {
        FDialogNode* runtimeNode = _WorkingAsset->GetNode(t);
        UDialogGraphNode_Base* graphNode = IndexToGraphNode.FindRef(runtimeNode->ID);

        int outputPinIndex = -1;
        for(int index = 0; index < graphNode->GetAllPins().Num(); index++)
        {
            UEdGraphPin* pin = graphNode->GetPinAt(index);
            if(pin->Direction == EEdGraphPinDirection::EGPD_Input) continue;

            UDialogGraphNode_Base* toNode = IndexToGraphNode.FindRef(graphNode->GetPinInfo()[++outputPinIndex].NextID);
            if(pin != nullptr && toNode != nullptr) graphNode->TryConnectToNode(pin, toNode);
        }
    }
}

void FDialogAssetEditor::UpdateAssetFromGraph()
{
    TArray<UDialogGraphNode_Base*> nodes;
    _WorkingGraph->GetNodesOfClass(nodes);

    if(nodes.IsEmpty()) return;

    _WorkingAsset->Clear();

    TMap<FGuid, int> GuidToIndex;
    for(int i = 0; i < nodes.Num(); i++)
    {
        if(nodes[i]->GetNodeType() == ENodeType::DEFAULT) continue;
        if(nodes[i]->GetNodeType() == ENodeType::Start) continue;

        FDialogNode* runtimeNode = _WorkingAsset->CreateNewNode();
        GuidToIndex.Emplace(nodes[i]->NodeGuid, runtimeNode->ID);

        runtimeNode->NodeType = nodes[i]->GetNodeType();
        runtimeNode->editorData = FEditorData
                (
                    nodes[i]->GetPosition(),
                    nodes[i]->NodeComment
                );
    }

    for(int t = 0; t < nodes.Num(); t++)
    {
        UDialogGraphNode_Base* graphNode = nodes[t];
        
        if(graphNode->GetNodeType() == ENodeType::DEFAULT) continue;
        if(graphNode->GetNodeType() == ENodeType::Start)
        {
            if(graphNode->GetPinAt(0)->HasAnyConnections())
            {
                _WorkingAsset->SetStartNodeID(*GuidToIndex.Find(graphNode->GetPinAt(0)->LinkedTo[0]->GetOwningNode()->NodeGuid));
            }

            continue;
        }

        FDialogNode* runtimeNode = _WorkingAsset->GetNode(GuidToIndex[graphNode->NodeGuid]);
        runtimeNode->NextIDs.Reserve(graphNode->GetAllPins().Num() - 1);

        int outputPinIndex = -1; //since we iterate over ALL the pins and we have no guarantees that the input pin will be the first one, we keep track of which output pin we're looking at, not incrementing when it's an input pin
        for(int q = 0; q < graphNode->GetAllPins().Num(); q++)
        {
            const UEdGraphPin* pin = graphNode->GetAllPins()[q];

            if(pin->Direction == EEdGraphPinDirection::EGPD_Input) continue;
            outputPinIndex++;

            FPinInfo& info = runtimeNode->NextIDs.AddDefaulted_GetRef();
            info.Title = graphNode->GetPinInfo()[outputPinIndex].Title;

            if(!pin->HasAnyConnections()) info.NextID = -1;
            else info.NextID = *GuidToIndex.Find(pin->LinkedTo[0]->GetOwningNode()->NodeGuid);
        }
    }
}
