#include <EditorGraph/GraphNodes/DialogGraphNode_Base.h>
#include <Framework/Commands/UIAction.h>
#include <ToolMenu.h>
#include <EdGraph/EdGraphPin.h>
#include <DialogNode.h>

void UDialogGraphNode_Base::SetupNode(UEdGraphPin* fromPin, const FEditorData* nodeData, bool bLoading)
{
    CreateNewGuid();

    NodePosX = nodeData->Position.X;
    NodePosY = nodeData->Position.Y;

    NodeComment = nodeData->Comment;
    bCommentBubbleVisible = nodeData->Comment != "" ? true : false;

    UEdGraphPin* inputPin = SetupNodePins(fromPin, nodeData->OutputPinCount);

    if(fromPin != nullptr && inputPin != nullptr)
    {
        if(bLoading) fromPin->LinkedTo.Add(inputPin);
        else GetSchema()->TryCreateConnection(fromPin, inputPin);
    }
}

void UDialogGraphNode_Base::GetNodeContextMenuActions(UToolMenu* menu, UGraphNodeContextMenuContext* context) const
{
    FToolMenuSection& section = menu->AddSection(TEXT("Dialog node section"), FText::FromString(TEXT("Dialog node actions")));
    AddMenuActions(&section);
}

void UDialogGraphNode_Base::DeleteNode()
{
    GetGraph()->RemoveNode(this);
}

UEdGraphPin* UDialogGraphNode_Base::CreateCustomPin(EEdGraphPinDirection direction, FName name) 
{
    FName category = (direction == EEdGraphPinDirection::EGPD_Input) ? TEXT("Prev") : TEXT("Next");
    UEdGraphPin* pin = CreatePin(direction, category, name);
    return pin;
}
