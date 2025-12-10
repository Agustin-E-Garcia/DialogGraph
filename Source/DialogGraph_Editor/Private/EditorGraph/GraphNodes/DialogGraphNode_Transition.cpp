#include <EditorGraph/GraphNodes/DialogGraphNode_Transition.h>

UDialogGraphNode_Transition::UDialogGraphNode_Transition() : UEdGraphNode()
{
    CreatePin(EEdGraphPinDirection::EGPD_Input, TEXT("Input"), TEXT(""));
    CreatePin(EEdGraphPinDirection::EGPD_Output, TEXT("Output"),TEXT(""));
}
