#pragma once

#include <Styling/SlateStyle.h>

struct FDialogPinFactory;
struct FChoicePinFactory;
struct FDialogLineNodeFactory;

class DialogStyleSet
{
public:
    DialogStyleSet();
    ~DialogStyleSet();

    TSharedPtr<FSlateStyleSet> GetStyleSet() { return _StyleSet; }

private:
    TSharedPtr<FSlateStyleSet> _StyleSet;
    TSharedPtr<FDialogPinFactory> _PinFactory;
    TSharedPtr<FChoicePinFactory> _ChoicePinFactory;
    TSharedPtr<FDialogLineNodeFactory> _LineNodeFactory;
};
