#pragma once

#include "CoreMinimal.h"

#include "DialogAssetEditorTypes.generated.h"

class FName;

UCLASS()
class UDialogAssetEditorTypes : public UObject
{
    GENERATED_UCLASS_BODY()

public:
    static const FName PinCategory_SingleNode;
    static const FName PinCategory_MultipleNodes;
    static const FName PinSubCategory_ChoiceNode;
};
