#include <EditorGraph/GraphNodes/DialogGraphNode_Line.h>
#include <ToolMenuSection.h>

UEdGraphPin* UDialogGraphNode_Line::SetupNodePins()
{
    UEdGraphPin* inputPin = CreateCustomPin(EEdGraphPinDirection::EGPD_Input, TEXT("prev"));
    CreateCustomPin(EEdGraphPinDirection::EGPD_Output, TEXT("next"));

    return inputPin;
}

void UDialogGraphNode_Line::SetDialogLine(FString inText)
{
    if(_PinInfo.IsEmpty()) _PinInfo.AddDefaulted();

    _PinInfo[0].Title = inText;
}

FString UDialogGraphNode_Line::GetDialogLine()
{
    if(_PinInfo.IsEmpty()) return "";

    return _PinInfo[0].Title;
}
