#pragma once

#include "CoreMinimal.h"
#include "DialogAssetGraphNode.h"
#include "DialogAssetGraphNode_Task.generated.h"

UCLASS()
class UDialogAssetGraphNode_Task : public UDialogAssetGraphNode
{
    GENERATED_UCLASS_BODY()

public:
    //~ Begin UDialogAssetGraphNode interface
    virtual EDialogNodeType GetNodeType() const override;
    virtual void ParseToRuntime(FDialogNode* RuntimeNode, const TMap<FGuid, int>& GuidToIndex) const override;
    //~ End UDialogAssetGraphNode interface

    //~ Begin UEdGraphNode interface
    virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
    virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override;
    virtual FLinearColor GetNodeTitleColor() const override;
    //~ End UEdGraphNode interface

    void SetFunctionData(TObjectPtr<UClass> FunctionClass, FName FunctionName);

private:
    UPROPERTY(VisibleAnywhere, Category="Task")
    FBindedFunctionData TaskFunctionData;
};
