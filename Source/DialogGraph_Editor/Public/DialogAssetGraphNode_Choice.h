#pragma once

#include "CoreMinimal.h"
#include "DialogAssetGraphNode_Line.h"
#include "DialogAssetGraphNode_Choice.generated.h"

UCLASS()
class UDialogAssetGraphNode_Choice : public UDialogAssetGraphNode_Line
{
    GENERATED_UCLASS_BODY()

public:
    //~ Begin UDialogAssetGraphNode interface
    virtual EDialogNodeType GetNodeType() const override;
    //~ End UDialogAssetGraphNode interface

    //~ Begin UEdGraphNode interface
    virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
    virtual FLinearColor GetNodeTitleColor() const override;
    virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override;
    //~ End UEdGraphNode interface
};
