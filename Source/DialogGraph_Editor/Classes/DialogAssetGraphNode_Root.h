#pragma once

#include "CoreMinimal.h"
#include "DialogAssetGraphNode.h"
#include "DialogAssetGraphNode_Root.generated.h"

UCLASS()
class UDialogAssetGraphNode_Root : public UDialogAssetGraphNode
{
    GENERATED_UCLASS_BODY()

public:
    virtual EDialogNodeType GetNodeType() const override { return EDialogNodeType::Start; }

    //~ Begin UEdGraphNode interface
    virtual void AllocateDefaultPins() override;
    virtual FLinearColor GetNodeTitleColor() const override;
    virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
    virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString(TEXT("On Dialog Start")); }
    virtual bool CanUserDeleteNode() const override;
    //~ End UEdGraphNode interface
};
