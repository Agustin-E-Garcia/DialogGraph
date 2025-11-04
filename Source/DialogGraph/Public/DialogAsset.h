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
    FDialogNode* CreateNewNode(NodeType nodeType);
    FDialogNode* GetNode(int ID);
    bool DeleteNode(int ID);

    FDialogNode* GetOrAddNode(int ID, NodeType nodeType);
    bool IsEmpty() { return DialogNodes.IsEmpty(); }

    int GetStartNodeID() { return StartNode; }

private:
    UPROPERTY(VisibleAnywhere)
    int _NextNodeID = 0;

    UPROPERTY(VisibleAnywhere)
    TMap<int, FDialogNode> DialogNodes;

    UPROPERTY(VisibleAnywhere)
    int StartNode = -1;
};
