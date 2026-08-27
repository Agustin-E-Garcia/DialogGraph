#include <EditorGraph/GraphNodes/DialogGraphNode_Start.h>

UEdGraphPin* UDialogGraphNode_Start::SetupNodePins()
{
    CreateCustomPin(EEdGraphPinDirection::EGPD_Output, TEXT("next"));
    return nullptr;
}
