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
};
