#pragma once

#include <CoreMinimal.h>
#include <EditorGraph/GraphNodes/DialogGraphNode_Base.h>
#include <DialogGraphNode_Choice.generated.h>

UCLASS()
class UDialogGraphNode_Choice : public UDialogGraphNode_Base
{
    GENERATED_BODY()

public:
    virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString(TEXT("Dialog Choice")); }
    virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor(FColor::Yellow); }

protected:
    virtual void AddMenuActions(FToolMenuSection* section) const override;
    virtual UEdGraphPin* SetupNodePins(UEdGraphPin* fromPin, int outputCount = 1) override;

private:
    void AddPinAction();
    void RemovePinAction();
};
