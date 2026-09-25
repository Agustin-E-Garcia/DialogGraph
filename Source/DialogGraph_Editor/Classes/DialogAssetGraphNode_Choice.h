#pragma once

#include "CoreMinimal.h"
#include "DialogAssetGraphNode_Line.h"
#include "DialogAssetGraphNode_Choice.generated.h"

UCLASS()
class UDialogAssetGraphNode_Choice : public UDialogAssetGraphNode_Line
{
    GENERATED_UCLASS_BODY()

public:
    virtual EDialogNodeType GetNodeType() const override { return EDialogNodeType::Choice; }

    //~ Begin UEdGraphNode interface
    virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
    virtual FLinearColor GetNodeTitleColor() const override;
    virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString(TEXT("Choice")); }
    //~ End UEdGraphNode interface
};
