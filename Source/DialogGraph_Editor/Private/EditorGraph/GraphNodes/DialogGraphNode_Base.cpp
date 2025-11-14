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
        if(bLoading)
        {
            inputPin->LinkedTo.Add(fromPin);
            fromPin->LinkedTo.Add(inputPin);
        }
        else GetSchema()->TryCreateConnection(fromPin, inputPin);
    }
}

void UDialogGraphNode_Base::GetNodeContextMenuActions(UToolMenu* menu, UGraphNodeContextMenuContext* context) const
{
    FToolMenuSection& section = menu->AddSection(TEXT("Dialog node section"), FText::FromString(TEXT("Dialog node actions")));
    AddMenuActions(&section);
}


void UDialogGraphNode_Base::AutowireNewNode(UEdGraphPin* fromPin)
{
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

UEdGraphPin* UDialogGraphNode_Base::CreateCustomPin(EEdGraphPinDirection direction, FName name) 
{
    FName category = (direction == EEdGraphPinDirection::EGPD_Input) ? TEXT("Prev") : TEXT("Next");
    FName subcategory = TEXT("DialogPin");

    UEdGraphPin* pin = CreatePin(direction, category, name);
    pin->PinType.PinSubCategory = subcategory;

    return pin;
}
