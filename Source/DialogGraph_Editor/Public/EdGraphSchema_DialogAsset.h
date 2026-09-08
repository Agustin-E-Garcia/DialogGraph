#pragma once

#include "Framework/Commands/UICommandInfo.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/ObjectMacros.h"
#include "EdGraph/EdGraphSchema.h"
#include "EdGraphSchema_DialogAsset.generated.h"

struct FGraphNodeClassHelper;

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
};
