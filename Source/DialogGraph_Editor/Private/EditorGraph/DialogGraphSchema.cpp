#include "EdGraph/EdGraphSchema.h"
#include <EditorGraph/DialogGraphSchema.h>
#include <EditorGraph/GraphNodes/DialogGraphNode_Base.h>
#include <EditorGraph/GraphNodes/DialogGraphNode_Choice.h>
#include <EditorGraph/GraphNodes/DialogGraphNode_Line.h>
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

    if(a == nullptr || b == nullptr) return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("At least one Pin is null"));
    if(a->Direction == b->Direction) return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Inputs can only connect to outputs"));

    UDialogGraphNode_Base* nodeA = Cast<UDialogGraphNode_Base>(a->GetOwningNode());
    UDialogGraphNode_Base* nodeB = Cast<UDialogGraphNode_Base>(b->GetOwningNode());

    if(a->Direction == EEdGraphPinDirection::EGPD_Input && nodeA->GetNodeType() == ENodeType::Choice) 
        return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));

    if(b->Direction == EEdGraphPinDirection::EGPD_Input && nodeB->GetNodeType() == ENodeType::Choice) 
        return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));

    return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT(""));
}

// ===== NodeActions functions =====

UEdGraphNode* FNewChoiceNodeAction::PerformAction(UEdGraph* parentGraph, UEdGraphPin* fromPin, const FVector2D location, bool bSelectNewNode)
{
    UDialogGraphNode_Base* result = NewObject<UDialogGraphNode_Choice>(parentGraph);
    FEditorData editorData(FVector2f(location.X, location.Y), "");
    result->SetupNode(&editorData);
    result->TryConnectToNode(fromPin, result);

    parentGraph->Modify();
    parentGraph->AddNode(result, true, true);

    return result;
}

UEdGraphNode* FNewLineNodeAction::PerformAction(UEdGraph* parentGraph, UEdGraphPin* fromPin, const FVector2D location, bool bSelectNewNode)
{
    UDialogGraphNode_Base* result = NewObject<UDialogGraphNode_Line>(parentGraph);
    FEditorData editorData(FVector2f(location.X, location.Y), "");
    result->SetupNode(&editorData);
    result->TryConnectToNode(fromPin, result);

    parentGraph->Modify();
    parentGraph->AddNode(result, true, true);

    return result;
}
