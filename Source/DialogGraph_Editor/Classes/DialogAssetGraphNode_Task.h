#pragma once

#include "CoreMinimal.h"
#include "DialogAssetGraphNode.h"
#include "StructUtils/PropertyBag.h"
#include "DialogAssetGraphNode_Task.generated.h"

UCLASS()
class UDialogAssetGraphNode_Task : public UDialogAssetGraphNode
{
    GENERATED_UCLASS_BODY()

public:
    virtual EDialogNodeType GetNodeType() const override { return EDialogNodeType::Task; }

    //~ Begin UEdGraphNode interface
    virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
    virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromName(BindedFunctionName); }
    FLinearColor GetNodeTitleColor() const override;
    //~ End UEdGraphNode interface

    virtual void ParseToRuntime(FDialogNode* RuntimeNode, const TMap<FGuid, int>& GuidToIndex) const override;

    void SetFunctionData(TObjectPtr<UClass> FunctionClass, FName FunctionName);

private:
    UPROPERTY(VisibleAnywhere, Category="Task")
    TObjectPtr<UClass> BindedFunctionClass;

    UPROPERTY(VisibleAnywhere, Category="Task")
    FName BindedFunctionName;

    UPROPERTY(EditAnywhere, Category="Task")
    FInstancedPropertyBag Parameters;
};
