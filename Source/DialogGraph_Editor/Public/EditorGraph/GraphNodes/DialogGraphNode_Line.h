#pragma once

#include "Internationalization/Text.h"
#include <CoreMinimal.h>
#include <EditorGraph/GraphNodes/DialogGraphNode_Base.h>
#include <DialogGraphNode_Line.generated.h>

UCLASS()
class UDialogGraphNode_Line : public UDialogGraphNode_Base
{
    GENERATED_BODY()

public:
    virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString(TEXT("Dialog Line")); }
    virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor(FColor::Blue); }

    void SetDialogLine(FText inText) { _DialogLine = inText; }
    FText GetDialogLine() { return _DialogLine; }

protected:
    virtual UEdGraphPin* SetupNodePins(UEdGraphPin* fromPin, int outputCount = 1) override;

    FText _DialogLine;
};
