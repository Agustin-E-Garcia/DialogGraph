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

    virtual NodeType GetNodeType() const override { return NodeType::Choice; }

    void SetPinOption(FGuid pinGuid, FString optionTitle);

protected:
    virtual void AddMenuActions(FToolMenuSection* section) const override;
    virtual UEdGraphPin* SetupNodePins() override;

private:
    void AddPinAction();
    void RemovePinAction();
};
