#pragma once

#include "CoreMinimal.h"
#include "DialogAssetGraphNode.h"
#include "DialogAssetGraphNode_Prompt.generated.h"

UCLASS()
class UDialogAssetGraphNode_Prompt : public UDialogAssetGraphNode
{
    GENERATED_UCLASS_BODY()

public:
    //~ Begin UDialogAssetGraphNode interface
    virtual EDialogNodeType GetNodeType() const override;
    virtual void ParseToRuntime(FDialogNode* RuntimeNode, const TMap<FGuid, int>& GuidToIndex) const override;
    //~ End UDialogAssetGraphNode interface

    //~ Begin UEdGraphNode interface
    virtual void AllocateDefaultPins() override;
    virtual FLinearColor GetNodeTitleColor() const override;
    virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
    virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override;
    //~ End UEdGraphNode interface

};
