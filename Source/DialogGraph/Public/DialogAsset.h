#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DialogNode.h"
#include "DialogGraphFunctionLibrary.h"
#include "DialogAsset.generated.h"

UCLASS(BlueprintType)
class DIALOGGRAPH_API UDialogAsset : public UDataAsset
{
    GENERATED_UCLASS_BODY()

public:
    FDialogNode* CreateNewNode();
    FDialogNode* GetNode(int ID);
    const FDialogNode* GetNode(int ID) const;
    FDialogNode* GetOrAddNode(int ID);
    void Clear();

    int GetStartNodeID() const { return StartNodeID; }
    void SetStartNodeID(int index) { StartNodeID = index; }

    int GetNodeCount() const { return DialogNodes.Num(); }
    bool IsEmpty() const { return DialogNodes.IsEmpty(); }

    UPROPERTY(EditAnywhere)
    TArray<TSubclassOf<UDialogGraphFunctionLibrary>> RegisteredLibraries;

#if WITH_EDITORONLY_DATA
    UPROPERTY()
    TObjectPtr<class UEdGraph> DialogGraph;
#endif

private:
    UPROPERTY(VisibleAnywhere) 
    TArray<FDialogNode> DialogNodes;

    UPROPERTY(VisibleAnywhere)
    int StartNodeID = -1;

};
