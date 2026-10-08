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
    void SetStartNodeID(int index);

    FDialogNode* GetNode(int ID);
    const FDialogNode* GetNode(int ID) const;
    int GetStartNodeID() const;
    int GetNodeCount() const;

    FDialogNode* GetOrAddNode(int ID);

    bool IsEmpty() const;
    void Clear();

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
