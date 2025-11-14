#include <EditorGraph/GraphNodes/DialogGraphNode_Line.h>
#include <ToolMenuSection.h>

UEdGraphPin* UDialogGraphNode_Line::SetupNodePins(UEdGraphPin* fromPin, int outputCount)
{
    UEdGraphPin* inputPin = CreateCustomPin(EEdGraphPinDirection::EGPD_Input, TEXT("prev"));
    CreateCustomPin(EEdGraphPinDirection::EGPD_Output, TEXT("next"));

    return inputPin;
}
