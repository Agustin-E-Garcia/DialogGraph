#pragma once

#include <EdGraph/EdGraph.h>
#include <EdGraph/EdGraphSchema.h>
#include <DialogGraphSchema.generated.h>

UCLASS()
class UDialogGraphSchema : public UEdGraphSchema
{
    GENERATED_BODY()
public:
    virtual void GetGraphContextActions(FGraphContextMenuBuilder& contextMenuBuilder) const override;
};

USTRUCT()
struct FNewChoiceNodeAction : public FEdGraphSchemaAction
{
    GENERATED_BODY()

public:
    FNewChoiceNodeAction() {}
    FNewChoiceNodeAction(FText inNodeCategory, FText inMenuDesc, FText inTooltip, const int32 grouping) : 
    FEdGraphSchemaAction(inNodeCategory, inMenuDesc, inTooltip, grouping) {}

    virtual UEdGraphNode* PerformAction(UEdGraph* parentGraph, UEdGraphPin* fromPin, const FVector2D location, bool bSelectNewNode = true);
};

USTRUCT()
struct FNewLineNodeAction : public FEdGraphSchemaAction
{
    GENERATED_BODY()
public:
    FNewLineNodeAction() {}
    FNewLineNodeAction(FText inNodeCategory, FText inMenuDesc, FText inTooltip, const int32 grouping) :
                       FEdGraphSchemaAction(inNodeCategory, inMenuDesc, inTooltip, grouping) {}

    virtual UEdGraphNode* PerformAction(UEdGraph* parentGraph, UEdGraphPin* fromPin, const FVector2D location, bool bSelectNewNode = true);
};
