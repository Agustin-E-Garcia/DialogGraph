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

    virtual ENodeType GetNodeType() const override { return ENodeType::Line; }

    void SetDialogLine(FString inText);
    FString GetDialogLine();

protected:
    virtual UEdGraphPin* SetupNodePins() override;
};
