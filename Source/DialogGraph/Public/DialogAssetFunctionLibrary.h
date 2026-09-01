#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DialogNode.h"
#include "DialogAssetFunctionLibrary.generated.h"

class UDialogAsset;

UENUM(BlueprintType)
enum class EDialogFlowResult : uint8
{
    HasNext,
    End
};

UCLASS()
class DIALOGGRAPH_API UDialogAssetFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dialog") static void GetNodeType(const FDialogNode& Node, ENodeType& OutType);
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dialog") static void GetLineText(const FDialogNode& Node, FText& OutText);
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dialog") static void GetChoiceOptions(const FDialogNode& Node, TArray<FPinInfo>& OutOptions);
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dialog") static void GetOptionText(const FPinInfo& Option, FText& OutText);

    UFUNCTION(BlueprintCallable, Category = "Dialog", meta = (ExpandEnumAsExecs = "Branches")) static void GetStartingNode(UDialogAsset* Asset, FDialogNode& OutStartNode, int32& OutStartIndex, EDialogFlowResult& Branches);
    UFUNCTION(BlueprintCallable, Category = "Dialog", meta = (ExpandEnumAsExecs = "Branches", AdvancedDisplay = "ChoiceIndex")) static void AdvanceDialog(UDialogAsset* Asset, int32 CurrentIndex, int32 ChoiceIndex, FDialogNode& OutNextNode, int32& OutNextIndex, EDialogFlowResult& Branches);
};
