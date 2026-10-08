#pragma once

#include "CoreMinimal.h"
#include "DialogAssetGraphNode.h"
#include "DialogAssetGraphNode_Line.generated.h"

UCLASS()
class UDialogAssetGraphNode_Line : public UDialogAssetGraphNode
{
    GENERATED_UCLASS_BODY()

public:

    //~ Begin UDialogAssetGraphNode interface
    virtual EDialogNodeType GetNodeType() const override;
    virtual void ParseToRuntime(FDialogNode* RuntimeNode, const TMap<FGuid, int>& GuidToIndex) const override;
    virtual bool CanUserAddCondition() const override;
    //~ End UDialogAssetGraphNode interface

    //~ Begin UEdGraphNode interface
    virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
    virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override;
    //~ End UEdGraphNode interface

    FText GetDialogLine() const;
    void SetDialogLine(const FText& InDialogLine);

private:
    UPROPERTY(EditAnywhere)
    FText DialogLine;

};
