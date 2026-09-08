#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "DialogAssetGraph.generated.h"

class UDialogAssetGraphNode_Root;

UCLASS(MinimalAPI)
class UDialogAssetGraph : public UEdGraph
{
    GENERATED_UCLASS_BODY()

    UPROPERTY()
    TSubclassOf<UDialogAssetGraphNode_Root> RootNodeClass;

    void OnCreated();
    void OnLoaded();
    void OnSave();

    void UpdateAsset();
};
