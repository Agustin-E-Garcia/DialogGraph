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
    virtual EDialogNodeType GetNodeType() const { return EDialogNodeType::DEFAULT; }

    //~ Begin UEdGraphNode Interface
    virtual void AllocateDefaultPins() override;
    virtual FLinearColor GetNodeTitleColor() const override;
    virtual bool ShowPaletteIconOnNode() const override;
    virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
    virtual bool CanUserDeleteNode() const override;
    //~ End UEdGraphNode Interface

    virtual void ParseToRuntime(FDialogNode* RuntimeNode, const TMap<FGuid, int>& GuidToIndex) const {}
    virtual bool CanUserAddCondition() const { return false; }

    void AddCondition(TObjectPtr<UClass> InClass, FName InName);

    bool HasConditions() const { return !Conditions.IsEmpty(); }
    void SetVisualGraphNode(SGraphNode* GraphNode) { VisualGraphNode = GraphNode; }


protected:
    SGraphNode* VisualGraphNode = nullptr;
    void InitializeBindedFunction(FBindedFunctionData& FunctionData);

private:
    UPROPERTY(EditAnywhere)
    TArray<FBindedFunctionData> Conditions;
};
