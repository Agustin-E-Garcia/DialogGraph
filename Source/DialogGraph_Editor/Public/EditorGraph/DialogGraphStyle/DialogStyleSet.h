#pragma once

#include <Styling/SlateStyle.h>
#include <EditorGraph/DialogGraphStyle/DialogTransitionNodeStyle.h>

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
    TSharedPtr<FDialogLineNodeFactory> _NodeLineFactory;
    TSharedPtr<FDialogTransitionNodeFactory> _NodeTransitionFactory;
};
