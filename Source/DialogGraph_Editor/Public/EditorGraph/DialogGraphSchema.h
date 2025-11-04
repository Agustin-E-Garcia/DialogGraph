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
    virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* a, const UEdGraphPin* b) const override;
};

USTRUCT()
struct FNewNodeBaseAction : public FEdGraphSchemaAction
{
    GENERATED_BODY()

public:
    FNewNodeBaseAction() {}
    FNewNodeBaseAction(FText inMenuDesc, FText inTooltip) : FEdGraphSchemaAction(GetNodeCategory(), inMenuDesc, inTooltip, GetGrouping()) {}

protected:
    FText GetNodeCategory() { return FText::FromString(TEXT("New Dialog Node")); }
    virtual FText GetMenuDesc() { return FText::FromString(TEXT("New Node")); }
    virtual FText GetTooltip() { return FText::FromString(TEXT("Creates new node")); }
    const int32 GetGrouping() { return 0; }
};

USTRUCT()
struct FNewChoiceNodeAction : public FNewNodeBaseAction
{
    GENERATED_BODY()

public:
    FNewChoiceNodeAction() : FNewNodeBaseAction(GetMenuDesc(), GetTooltip()) {}

    virtual UEdGraphNode* PerformAction(UEdGraph* parentGraph, UEdGraphPin* fromPin, const FVector2D location, bool bSelectNewNode = true);

protected:
    virtual FText GetMenuDesc() override { return FText::FromString(TEXT("New choice node")); }
    virtual FText GetTooltip() override { return FText::FromString(TEXT("Creates new choice node")); }
};

USTRUCT()
struct FNewLineNodeAction : public FNewNodeBaseAction
{
    GENERATED_BODY()
public:
    FNewLineNodeAction() : FNewNodeBaseAction(GetMenuDesc(), GetTooltip()) {}

    virtual UEdGraphNode* PerformAction(UEdGraph* parentGraph, UEdGraphPin* fromPin, const FVector2D location, bool bSelectNewNode = true);

protected:
    virtual FText GetMenuDesc() override { return FText::FromString(TEXT("New line node")); }
    virtual FText GetTooltip() override { return FText::FromString(TEXT("Creates new line node")); }
};
