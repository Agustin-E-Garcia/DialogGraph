#pragma once

#include "EdGraph/EdGraphNode.h"
#include "Framework/Commands/UICommandInfo.h"
#include "Templates/UnrealTemplate.h"
#include "Types/SlateEnums.h"
#include "UObject/Class.h"
#include "UObject/ObjectMacros.h"
#include "EdGraph/EdGraphSchema.h"
#include "EdGraphSchema_DialogAsset.generated.h"

struct FGraphNodeClassHelper;
struct FGraphActionListBuilderBase;

/** Action to add a comment to the graph */
USTRUCT()
struct FDialogSchemaAction_AddComment : public FEdGraphSchemaAction
{
    GENERATED_BODY()

    FDialogSchemaAction_AddComment() : FEdGraphSchemaAction() {}
    FDialogSchemaAction_AddComment(FText InDescription, FText InToolTip) : FEdGraphSchemaAction(FText(), MoveTemp(InDescription), MoveTemp(InToolTip), 0) {}

    //~ Begin FEdGraphSchemaAction interface
    virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode = true) override final;
    //~ End FEdGraphSchemaAction interface
};

/** Action to add a node to the graph */
USTRUCT()
struct FDialogSchemaAction_NewNode : public FEdGraphSchemaAction
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    TObjectPtr<class UDialogAssetGraphNode> NodeTemplate;

    FDialogSchemaAction_NewNode() : FEdGraphSchemaAction(), NodeTemplate(nullptr) {}
    FDialogSchemaAction_NewNode(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping) : FEdGraphSchemaAction(MoveTemp(InNodeCategory),MoveTemp(InMenuDesc), MoveTemp(InToolTip), InGrouping), NodeTemplate(nullptr) {}

    //~ Begin FEdGraphSchemaAction Interface
    virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode = true) override;
    virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
    //~ End FEdGraphSchemaAction Interface
};

/** Action to add a condition to a choice pin */
USTRUCT()
struct FDialogSchemaAction_AddCondition : public FEdGraphSchemaAction
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    TObjectPtr<UClass> BindedFunctionClass;

    UPROPERTY()
    FName BindedFunctionName;

    FDialogSchemaAction_AddCondition() : FEdGraphSchemaAction(), BindedFunctionClass(nullptr), BindedFunctionName("") {}
    FDialogSchemaAction_AddCondition(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping) : FEdGraphSchemaAction(MoveTemp(InNodeCategory), MoveTemp(InMenuDesc), MoveTemp(InToolTip), InGrouping), BindedFunctionClass(nullptr), BindedFunctionName("") {}

    //~ Begin FEdGraphSchemaAction Interface
    virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode = true) override;
    //~ End FEdGraphSchemaAction Interface
};

UCLASS()
class UEdGraphSchema_DialogAsset : public UEdGraphSchema
{
    GENERATED_UCLASS_BODY()

public:

    //~ Begin UEdGraphSchema Interface
    virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
    virtual void GetGraphContextActions(FGraphContextMenuBuilder& contextMenuBuilder) const override;
    virtual void GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
    virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* a, const UEdGraphPin* b) const override;
    //~ End UEdGraphSchema Interface

    static TSharedPtr<FDialogSchemaAction_NewNode> AddNewNodeAction(FGraphActionListBuilderBase& ContextMenuBuilder, const FText& Category, const FText& MenuDesc, const FText& Tooltip);

protected:
    virtual FGraphNodeClassHelper& GetClassCache() const;

    TSharedPtr<FUICommandInfo> RemovePinCommand;

private:
    void CreateAddConditionSubMenu(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const;
    void CollectAllActions(FGraphActionListBuilderBase& OutAllActions, UGraphNodeContextMenuContext* Context) const;
    void OnActionSelected(const TArray<TSharedPtr<FEdGraphSchemaAction>>& SelectedAction, ESelectInfo::Type InSelectionType, UGraphNodeContextMenuContext* Context) const;
};
