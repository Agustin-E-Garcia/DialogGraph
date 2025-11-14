#include <EditorGraph/GraphNodes/DialogGraphNode_Choice.h>
#include <EdGraph/EdGraph.h>
#include <ToolMenuSection.h>

void UDialogGraphNode_Choice::AddMenuActions(FToolMenuSection* section) const
{
    UDialogGraphNode_Base::AddMenuActions(section);

    section->AddMenuEntry
        (
            TEXT("AddPinEntry"),
            FText::FromString(TEXT("Add Pin")),
            FText::FromString(TEXT("Creates a new pin")),
            FSlateIcon(TEXT("CustomAssetEditorStyle"), TEXT("CustomAssetEditor.NodeAddPinIcon")),
            FUIAction(FExecuteAction::CreateUObject(const_cast<UDialogGraphNode_Choice*>(this), &UDialogGraphNode_Choice::AddPinAction))
        );

    section->AddMenuEntry
        (
            TEXT("DeletePinEntry"),
            FText::FromString(TEXT("Delete Pin")),
            FText::FromString(TEXT("Deletes the last pin")),
            FSlateIcon(TEXT("CustomAssetEditorStyle"), TEXT("CustomAssetEditor.NodeDeletePinIcon")),
            FUIAction(FExecuteAction::CreateUObject(const_cast<UDialogGraphNode_Choice*>(this), &UDialogGraphNode_Choice::RemovePinAction))
        );
}

void UDialogGraphNode_Choice::AddPinAction()
{
    CreateCustomPin(EEdGraphPinDirection::EGPD_Output, *FString::FromInt(GetOutputPinCount()));
    GetGraph()->NotifyGraphChanged();
    GetGraph()->Modify();
}

void UDialogGraphNode_Choice::RemovePinAction()
{
    UEdGraphPin* pin = GetPinAt(Pins.Num() - 1);
    if(pin->Direction == EEdGraphPinDirection::EGPD_Input) return;

    RemovePin(pin);
    GetGraph()->NotifyGraphChanged();
    GetGraph()->Modify();
}

UEdGraphPin* UDialogGraphNode_Choice::SetupNodePins(UEdGraphPin* fromPin, int outputCount)
{
    UEdGraphPin* inputPin = CreateCustomPin(EEdGraphPinDirection::EGPD_Input, TEXT("prev"));

    for (int i = 0; i < outputCount; i++)
    {
        CreateCustomPin(EEdGraphPinDirection::EGPD_Output, *FString::FromInt(i));
    }

    return inputPin;
}
