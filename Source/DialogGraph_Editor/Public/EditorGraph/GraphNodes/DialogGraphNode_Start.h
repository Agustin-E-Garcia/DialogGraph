#pragma once

#include <CoreMinimal.h>
#include <EditorGraph/GraphNodes/DialogGraphNode_Base.h>
#include <DialogGraphNode_Start.generated.h>

UCLASS()
class UDialogGraphNode_Start : public UDialogGraphNode_Base
{
    GENERATED_BODY()

public:
    virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString(TEXT("Dialog Start")); }
    virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor(FColor::Red); }

    virtual bool CanUserDeleteNode() const override { return false; }
protected:
    virtual UEdGraphPin* SetupNodePins(UEdGraphPin* fromPin, int outputCount = 1) override;
};
