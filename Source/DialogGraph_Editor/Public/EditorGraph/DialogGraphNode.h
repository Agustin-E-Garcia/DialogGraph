#pragma once

#include <CoreMinimal.h>
#include <EdGraph/EdGraph.h>
#include <DialogGraphNode.generated.h>

class UToolMenu;
class UGraphNodeContextMenuContext;

UCLASS()
class UDialogLineGraphNode : public UEdGraphNode
{
    GENERATED_BODY()

public:
    virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString(TEXT("Dialog Line")); }
    virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor(FColor::Blue); }
    virtual bool CanUserDeleteNode() const override { return true; }
};

UCLASS()
class UDialogChoiceGraphNode : public UEdGraphNode
{
    GENERATED_BODY()

public:
    virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString(TEXT("Dialog Choice")); }
    virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor(FColor::Red); }
    virtual bool CanUserDeleteNode() const override { return true; }
    virtual void GetNodeContextMenuActions(UToolMenu* menu, UGraphNodeContextMenuContext* context) const override;
};
