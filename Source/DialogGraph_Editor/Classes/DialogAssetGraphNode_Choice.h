#pragma once

#include "CoreMinimal.h"
#include "DialogAssetGraphNode.h"
#include "DialogAssetGraphNode_Choice.generated.h"

UCLASS(MinimalAPI)
class UDialogAssetGraphNode_Choice : public UDialogAssetGraphNode
{
    GENERATED_UCLASS_BODY()

public:
    virtual EDialogNodeType GetNodeType() const override { return EDialogNodeType::Choice; }

    //~ Begin UEdGraphNode Interface
    virtual void AllocateDefaultPins() override;
    virtual FLinearColor GetNodeTitleColor() const override;
    virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
    virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString(TEXT("Dialog Branch")); }
    //~ End UEdGraphNode Interface

    void AddPin();
    void RemovePin(UEdGraphPin* PinToRemove);
};
