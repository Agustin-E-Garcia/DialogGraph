#include <EditorGraph/DialogGraphSchema.h>
#include <EditorGraph/DialogGraphNode.h>

void UDialogGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& contextMenuBuilder) const
{
    TSharedPtr<FNewChoiceNodeAction> newChoiceAction(new FNewChoiceNodeAction(
        FText::FromString(TEXT("Nodes")),
        FText::FromString(TEXT("New Choice")),
        FText::FromString(TEXT("Adds a Dialog Choice node")),
        0
    ));

    TSharedPtr<FNewLineNodeAction> newLineAction(new FNewLineNodeAction(
        FText::FromString(TEXT("Nodes")),
        FText::FromString(TEXT("New Line")),
        FText::FromString(TEXT("Adds a Dialog line node")),
        0
    ));

    contextMenuBuilder.AddAction(newChoiceAction);
    contextMenuBuilder.AddAction(newLineAction);
}

UEdGraphNode* FNewChoiceNodeAction::PerformAction(UEdGraph* parentGraph, UEdGraphPin* fromPin, const FVector2D location, bool bSelectNewNode)
{
    UDialogChoiceGraphNode* result = NewObject<UDialogChoiceGraphNode>(parentGraph);
    result->NodePosX = location.X;
    result->NodePosY = location.Y;

    result->CreatePin(EEdGraphPinDirection::EGPD_Input, TEXT("Input"), TEXT("Prev Dialog"));
    result->CreatePin(EEdGraphPinDirection::EGPD_Output, TEXT("Output"), TEXT("Next Dialog"));

    parentGraph->Modify();
    parentGraph->AddNode(result, true, true);

    return result;
}

UEdGraphNode* FNewLineNodeAction::PerformAction(UEdGraph* parentGraph, UEdGraphPin* fromPin, const FVector2D location, bool bSelectNewNode)
{
    UDialogLineGraphNode* result = NewObject<UDialogLineGraphNode>(parentGraph);
    result->NodePosX = location.X;
    result->NodePosY = location.Y;

    result->CreatePin(EEdGraphPinDirection::EGPD_Input, TEXT("Input"), TEXT("Prev Dialog"));
    result->CreatePin(EEdGraphPinDirection::EGPD_Output, TEXT("Output"), TEXT("Choice #1"));

    parentGraph->Modify();
    parentGraph->AddNode(result, true, true);

    return result;
}
