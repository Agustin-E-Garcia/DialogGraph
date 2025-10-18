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
    bool CreateNewNode();
    bool DeleteNode(int ID);

    UPROPERTY(VisibleAnywhere)
    TMap<int, FDialogNode> DialogNodes;

private:
    UPROPERTY(VisibleAnywhere)
    int _NextNodeID = -1;
};
