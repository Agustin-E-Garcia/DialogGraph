#include <EditorGraph/GraphNodes/DialogGraphNode_Choice.h>
#include <EdGraph/EdGraph.h>
#include <EdGraph/EdGraphPin.h>
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
    _PinInfo.AddDefaulted();
    CreateCustomPin(EEdGraphPinDirection::EGPD_Output, "", "ChoicePin");
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

UEdGraphPin* UDialogGraphNode_Choice::SetupNodePins()
{
    UEdGraphPin* inputPin = CreateCustomPin(EEdGraphPinDirection::EGPD_Input, TEXT("prev"));

    if(_PinInfo.IsEmpty())
    {
        CreateCustomPin(EEdGraphPinDirection::EGPD_Output, "", "ChoicePin");
        _PinInfo.AddDefaulted();
        return inputPin;
    }

    for (int i = 0; i < _PinInfo.Num(); i++)
    {
        CreateCustomPin(EEdGraphPinDirection::EGPD_Output, *_PinInfo[i].Title, "ChoicePin");
    }

    return inputPin;
}

void UDialogGraphNode_Choice::SetPinOption(FGuid pinGuid, FString optionTitle)
{
    int outputIndex = -1;
    for(int i = 0; i < GetAllPins().Num(); i++)
    {
        if(GetAllPins()[i]->Direction == EEdGraphPinDirection::EGPD_Input) continue;
        outputIndex++;

        if(GetAllPins()[i]->PinId != pinGuid) continue;
        _PinInfo[outputIndex].Title = optionTitle;
    }
}
