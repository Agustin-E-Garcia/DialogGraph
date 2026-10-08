#pragma once

#include "CoreMinimal.h"
#include "DialogAssetGraphNode.h"
#include "DialogAssetGraphNode_Root.generated.h"

UCLASS()
class UDialogAssetGraphNode_Root : public UDialogAssetGraphNode
{
    GENERATED_UCLASS_BODY()

public:
    //~ Begin UDialogAssetGraphNode interface
    virtual EDialogNodeType GetNodeType() const override;
    //~ End UDialogAssetGraphNode interface

    //~ Begin UEdGraphNode interface
    virtual void AllocateDefaultPins() override;
    virtual FLinearColor GetNodeTitleColor() const override;
    virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
    virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override;
    virtual bool CanUserDeleteNode() const override;
    virtual bool CanDuplicateNode() const override;
    //~ End UEdGraphNode interface
};
