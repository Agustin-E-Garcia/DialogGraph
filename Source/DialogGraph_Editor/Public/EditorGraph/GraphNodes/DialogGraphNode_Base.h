#pragma once

#include <CoreMinimal.h>
#include <EdGraph/EdGraph.h>
#include <DialogNode.h>
#include <EdGraph/EdGraphPin.h>
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

    void SetupNode(const FEditorData* nodeData);
    bool TryConnectToNode(UEdGraphPin* fromPin, UDialogGraphNode_Base* toNode);

    virtual void GetNodeContextMenuActions(UToolMenu* menu, UGraphNodeContextMenuContext* context) const override;
    virtual UEdGraphPin* CreateCustomPin(EEdGraphPinDirection direction, FName name, FString subCategory = "DialogPin");

    virtual bool CanUserDeleteNode() const override { return true; }

    int GetOutputPinCount() const { return GetAllPins().Num() - 1; }
    virtual ENodeType GetNodeType() const { return ENodeType::DEFAULT; }

    TArray<FPinInfo>& GetPinInfo() { return _PinInfo; }
    void SetPinInfo(TArray<FPinInfo> info) { _PinInfo = info; }

    UEdGraphPin* GetInputPin() { return _InputPin; }

protected:
    virtual void AddMenuActions(FToolMenuSection* section) const;
    virtual UEdGraphPin* SetupNodePins() { return nullptr; }

    void DeleteNode();

    TArray<FPinInfo> _PinInfo;
    UEdGraphPin* _InputPin;
};
