#pragma once

#include <CoreMinimal.h>
#include <Engine/DataAsset.h>
#include <DialogNode.h>
#include <DialogAsset.generated.h>

UCLASS(BlueprintType)
class DIALOGGRAPH_API UDialogAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    FDialogNode* CreateNewNode();
    FDialogNode* GetNode(int ID);
    FDialogNode* GetOrAddNode(int ID);

    int GetNodeCount() { return DialogNodes.Num(); }
    bool IsEmpty() { return DialogNodes.IsEmpty(); }
    void Clear() { DialogNodes.Empty(); }

private:
    UPROPERTY(VisibleAnywhere) TArray<FDialogNode> DialogNodes;
};
