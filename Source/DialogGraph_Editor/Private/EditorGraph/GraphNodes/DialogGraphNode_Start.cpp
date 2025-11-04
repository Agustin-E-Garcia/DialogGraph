#include <EditorGraph/GraphNodes/DialogGraphNode_Start.h>

UEdGraphPin* UDialogGraphNode_Start::SetupNodePins(UEdGraphPin* fromPin, int outputCount)
{
    CreatePin(EEdGraphPinDirection::EGPD_Output, TEXT("Output"), TEXT("next"));
    return nullptr;
}
