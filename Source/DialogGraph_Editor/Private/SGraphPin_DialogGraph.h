#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphPin.h"
#include "SGraphPin.h"
#include "Types/SlateEnums.h"

class SGraphPin_DialogGraph : public SGraphPin
{
public:
    SLATE_BEGIN_ARGS(SGraphPin_DialogGraph) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj);

    //~ Begin SGraphPin interface
    virtual const FSlateBrush* GetPinIcon() const override;
    virtual FSlateColor GetPinColor() const override;
    //~ End SGraphPin interface

    const FSlateBrush* GetPinBorder() const;
    FSlateColor GetPinBorderColor() const;

    //FText GetTypeInValue();
    //void SetTypeInValue(const FText& NewTypeInValue, ETextCommit::Type Type);
    //const FSlateBrush* GetConditionLockImage();
};
