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
    TSharedRef<SWidget> GetLabelWidget(const FName& InPinLabelStyle) override;
    virtual FSlateColor GetPinColor() const override;
    //~ End SGraphPin interface

    FText GetTypeInValue();
    void SetTypeInValue(const FText& NewTypeInValue, ETextCommit::Type Type);
};
