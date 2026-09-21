#pragma once

#include "CoreMinimal.h"
#include "DialogAssetGraphNode.h"
#include "DialogAssetGraphNode_Choice.generated.h"

USTRUCT()
struct FChoicePinData
{
    GENERATED_USTRUCT_BODY()

    FChoicePinData() {}

    FText GetDefaultValueAsText() const { return FText::FromString(DefaultValue); }

    UPROPERTY(VisibleAnywhere)
    FString DefaultValue;

    UPROPERTY(EditAnywhere)
    TArray<FBindedFunctionData> Conditions;
};

UCLASS()
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

    virtual void ParseToRuntime(FDialogNode* RuntimeNode, const TMap<FGuid, int>& GuidToIndex) const override;

    const FChoicePinData* GetPinData(const FGuid PinID) const;
    void SetPinData(const FGuid PinID, FString InDefaultValue);
    void AddPinCondition(const FGuid PinID, TObjectPtr<UClass> InClass, FName InName);

    void AddPin();
    void RemovePin(UEdGraphPin* PinToRemove);

private:
    UPROPERTY(EditAnywhere)
    TMap<FGuid, FChoicePinData> GuidToPinData;
};
