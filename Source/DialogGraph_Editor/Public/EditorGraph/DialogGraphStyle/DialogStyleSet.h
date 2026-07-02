#pragma once

#include <Styling/SlateStyle.h>

struct FDialogPinFactory;
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
    TSharedPtr<FDialogLineNodeFactory> _NodeFactory;
};
