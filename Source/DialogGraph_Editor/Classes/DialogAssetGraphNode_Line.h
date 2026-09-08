#pragma once

#include "CoreMinimal.h"
#include "DialogAssetGraphNode.h"
#include "DialogAssetGraphNode_Line.generated.h"

UCLASS(MinimalAPI)
class UDialogAssetGraphNode_Line : public UDialogAssetGraphNode
{
    GENERATED_UCLASS_BODY()

public:
    virtual EDialogNodeType GetNodeType() const override { return EDialogNodeType::Line; }

    //~ Begin UEdGraphNode interface
    virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
    virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString(TEXT("Dialog Line")); }
    //~ End UEdGraphNode interface

    FText GetDialogLine() { return DialogLine; }
    void SetDialogLine(const FText& InDialogLine) { DialogLine = InDialogLine; }

private:
    UPROPERTY()
    FText DialogLine;
};
