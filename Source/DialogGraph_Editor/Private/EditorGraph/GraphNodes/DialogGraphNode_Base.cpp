#include <EditorGraph/GraphNodes/DialogGraphNode_Base.h>
#include <Framework/Commands/UIAction.h>
#include <ToolMenu.h>
#include <EdGraph/EdGraphPin.h>
#include <DialogNode.h>

void UDialogGraphNode_Base::SetupNode(const FEditorData* nodeData)
{
    CreateNewGuid();

    NodePosX = nodeData->Position.X;
    NodePosY = nodeData->Position.Y;

    NodeComment = nodeData->Comment;
    bCommentBubbleVisible = nodeData->Comment != "" ? true : false;

    _InputPin = SetupNodePins();
}

bool UDialogGraphNode_Base::TryConnectToNode(UEdGraphPin* fromPin, UDialogGraphNode_Base* toNode)
{
    return GetSchema()->TryCreateConnection(fromPin, toNode->GetInputPin());
}

void UDialogGraphNode_Base::GetNodeContextMenuActions(UToolMenu* menu, UGraphNodeContextMenuContext* context) const
{
    FToolMenuSection& section = menu->AddSection(TEXT("Dialog node section"), FText::FromString(TEXT("Dialog node actions")));
    AddMenuActions(&section);
}

void UDialogGraphNode_Base::AddMenuActions(FToolMenuSection* section) const
{
    section->AddMenuEntry
    (
        TEXT("DeleteEntry"),
        FText::FromString(TEXT("Delete Node")),
        FText::FromString(TEXT("Deletes the node")),
        FSlateIcon(TEXT("CustomAssetEditorStyle"), TEXT("CustomAssetEditor.NodeDeleteNodeIcon")),
        FUIAction(FExecuteAction::CreateUObject(const_cast<UDialogGraphNode_Base*>(this), &UDialogGraphNode_Base::DeleteNode))
    );
}

void UDialogGraphNode_Base::DeleteNode()
{
    GetGraph()->RemoveNode(this);
}

UEdGraphPin* UDialogGraphNode_Base::CreateCustomPin(EEdGraphPinDirection direction, FName name, FString subCategory)
{
    FName category = (direction == EEdGraphPinDirection::EGPD_Input) ? TEXT("Prev") : TEXT("Next");
    FName subcategory = FName(subCategory);

    UEdGraphPin* pin = CreatePin(direction, category, name);
    pin->PinType.PinSubCategory = subcategory;

    return pin;
}
