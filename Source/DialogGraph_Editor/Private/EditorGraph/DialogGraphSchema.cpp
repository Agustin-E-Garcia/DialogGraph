#include <EditorGraph/DialogGraphSchema.h>
#include <EditorGraph/GraphNodes/DialogGraphNode_Choice.h>
#include <EditorGraph/GraphNodes/DialogGraphNode_Line.h>
#include <EditorGraph/GraphNodes/DialogGraphNode_Start.h>
#include <DialogNode.h>

// ===== UDilaogGraphSchema functions

void UDialogGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& contextMenuBuilder) const
{
    TSharedPtr<FNewChoiceNodeAction> newChoiceAction(new FNewChoiceNodeAction());
    TSharedPtr<FNewLineNodeAction> newLineAction(new FNewLineNodeAction());

    contextMenuBuilder.AddAction(newChoiceAction);
    contextMenuBuilder.AddAction(newLineAction);
}

const FPinConnectionResponse UDialogGraphSchema::CanCreateConnection(const UEdGraphPin* a, const UEdGraphPin* b) const
{
    if(a == nullptr || b == nullptr) return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT(""));
    if(a->Direction == b->Direction) return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Inputs can only connect to outputs"));
    return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT(""));
}

// ===== NodeActions functions =====

UEdGraphNode* FNewChoiceNodeAction::PerformAction(UEdGraph* parentGraph, UEdGraphPin* fromPin, const FVector2D location, bool bSelectNewNode)
{
    UDialogGraphNode_Base* result = NewObject<UDialogGraphNode_Choice>(parentGraph);
    FEditorData editorData(1, FVector2f(location.X, location.Y), "");
    result->SetupNode(fromPin, &editorData);

    parentGraph->Modify();
    parentGraph->AddNode(result, true, true);

    return result;
}

UEdGraphNode* FNewLineNodeAction::PerformAction(UEdGraph* parentGraph, UEdGraphPin* fromPin, const FVector2D location, bool bSelectNewNode)
{
    UDialogGraphNode_Base* result = NewObject<UDialogGraphNode_Line>(parentGraph);
    FEditorData editorData(1, FVector2f(location.X, location.Y), "");
    result->SetupNode(fromPin, &editorData);

    parentGraph->Modify();
    parentGraph->AddNode(result, true, true);

    return result;
}
