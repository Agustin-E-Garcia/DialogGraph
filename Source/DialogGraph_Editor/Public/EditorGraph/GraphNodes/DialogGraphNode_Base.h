#pragma once

#include <CoreMinimal.h>
#include <EdGraph/EdGraph.h>
#include <DialogGraphNode_Base.generated.h>

class UToolMenu;
class UGraphNodeContextMenuContext;
struct FToolMenuSection;
struct FEditorData;

UCLASS()
class UDialogGraphNode_Base : public UEdGraphNode
{
    GENERATED_BODY()

public:
    UDialogGraphNode_Base() : UEdGraphNode() {}

    void SetupNode(UEdGraphPin* fromPin, const FEditorData* nodeData, bool bLoading = false);
    virtual void GetNodeContextMenuActions(UToolMenu* menu, UGraphNodeContextMenuContext* context) const override;
    virtual UEdGraphPin* CreateCustomPin(EEdGraphPinDirection direction, FName name);

    virtual bool CanUserDeleteNode() const override { return true; }
    int GetOutputPinCount() const { return GetAllPins().Num() - 1; }

    int runtimeNodeID = -1;

protected:
    virtual void AddMenuActions(FToolMenuSection* section) const;
    virtual void AutowireNewNode(UEdGraphPin* fromPin) override;
    virtual UEdGraphPin* SetupNodePins(UEdGraphPin* fromPin, int outputCount = 1) { return nullptr; }

    void DeleteNode();
};
