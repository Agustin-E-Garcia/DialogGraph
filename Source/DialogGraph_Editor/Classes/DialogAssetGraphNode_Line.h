#pragma once

#include "CoreMinimal.h"
#include "DialogAssetGraphNode.h"
#include "DialogAssetGraphNode_Line.generated.h"

UCLASS()
class UDialogAssetGraphNode_Line : public UDialogAssetGraphNode
{
    GENERATED_UCLASS_BODY()

public:
    virtual EDialogNodeType GetNodeType() const override { return EDialogNodeType::Line; }

    //~ Begin UEdGraphNode interface
    virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
    virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString(TEXT("Dialog Line")); }
    //~ End UEdGraphNode interface

    virtual void ParseToRuntime(FDialogNode* RuntimeNode, const TMap<FGuid, int>& GuidToIndex) const override;

    FText GetDialogLine() const { return DialogLine; }
    void SetDialogLine(const FText& InDialogLine) { DialogLine = InDialogLine; }

    void AddCondition(TObjectPtr<UClass> InClass, FName InName);

    bool HasConditions() { return !Conditions.IsEmpty(); }

private:
    UPROPERTY(EditAnywhere)
    FText DialogLine;

    UPROPERTY(EditAnywhere)
    TArray<FBindedFunctionData> Conditions;
};
