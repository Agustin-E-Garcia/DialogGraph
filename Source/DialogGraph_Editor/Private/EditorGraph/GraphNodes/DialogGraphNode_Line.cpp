#include <EditorGraph/GraphNodes/DialogGraphNode_Line.h>
#include <ToolMenuSection.h>

UEdGraphPin* UDialogGraphNode_Line::SetupNodePins(UEdGraphPin* fromPin, int outputCount)
{
    UEdGraphPin* inputPin = CreatePin(EEdGraphPinDirection::EGPD_Input, TEXT("Input"), TEXT("prev"));
    CreatePin(EEdGraphPinDirection::EGPD_Output, TEXT("Output"), TEXT("next"));

    return inputPin;
}

void UDialogGraphNode_Line::AddMenuActions(FToolMenuSection* section) const
{
    section->AddMenuEntry
    (
        TEXT("DeleteEntry"),
        FText::FromString(TEXT("Delete Node")),
        FText::FromString(TEXT("Deletes the node")),
        FSlateIcon(TEXT("CustomAssetEditorStyle"), TEXT("CustomAssetEditor.NodeDeleteNodeIcon")),
        FUIAction(FExecuteAction::CreateUObject(const_cast<UDialogGraphNode_Line*>(this), &UDialogGraphNode_Line::DeleteNode))
    );
}
