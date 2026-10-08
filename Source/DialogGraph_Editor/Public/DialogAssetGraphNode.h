#pragma once

#include "CoreMinimal.h"
#include "DialogNode.h"
#include "EdGraph/EdGraphNode.h"
#include "DialogAssetGraphNode.generated.h"

class UEdGraphSchema;

UCLASS()
class UDialogAssetGraphNode : public UEdGraphNode
{
    GENERATED_UCLASS_BODY()

public:
    virtual EDialogNodeType GetNodeType() const;
    virtual void ParseToRuntime(FDialogNode* RuntimeNode, const TMap<FGuid, int>& GuidToIndex) const;
    virtual bool CanUserAddCondition() const;

    //~ Begin UEdGraphNode interface
    virtual void AllocateDefaultPins() override;
    virtual FLinearColor GetNodeTitleColor() const override;
    virtual bool ShowPaletteIconOnNode() const override;
    virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
    virtual bool CanUserDeleteNode() const override;
    virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
    //~ End UEdGraphNode interface

    void AddCondition(TObjectPtr<UClass> InClass, FName InName);
    TArray<FBindedFunctionData> GetConditionsData() const;

    bool HasConditions() const;
    const UEdGraphPin* GetOutputPin() const;


protected:
    UPROPERTY(EditAnywhere)
    TArray<FBindedFunctionData> Conditions;

    void InitializeBindedFunction(FBindedFunctionData& FunctionData);

};
