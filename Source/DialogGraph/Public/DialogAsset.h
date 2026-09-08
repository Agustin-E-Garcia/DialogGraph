#pragma once

#include "UObject/ObjectMacros.h"
#include <CoreMinimal.h>
#include <Engine/DataAsset.h>
#include <DialogNode.h>
#include <DialogAsset.generated.h>

UCLASS(BlueprintType)
class DIALOGGRAPH_API UDialogAsset : public UDataAsset
{
    GENERATED_UCLASS_BODY()

public:
    FDialogNode* CreateNewNode();
    FDialogNode* GetNode(int ID);
    FDialogNode* GetOrAddNode(int ID);
    void Clear();

    int GetStartNodeID() { return StartNodeID; }
    void SetStartNodeID(int index) { StartNodeID = index; }

    int GetNodeCount() { return DialogNodes.Num(); }
    bool IsEmpty() { return DialogNodes.IsEmpty(); }

#if WITH_EDITORONLY_DATA
    UPROPERTY()
    TObjectPtr<class UEdGraph> DialogGraph;
#endif

private:
    UPROPERTY(VisibleAnywhere) TArray<FDialogNode> DialogNodes;
    UPROPERTY(VisibleAnywhere) int StartNodeID = -1;
};
