#include <EditorGraph/DialogGraphStyle/DialogStyleSet.h>
#include <Interfaces/IPluginManager.h>
#include <EditorGraph/DialogGraphStyle/DialogPinStyle.h>
#include <EditorGraph/DialogGraphStyle/ChoicePinStyle.h>
#include <EditorGraph/DialogGraphStyle/DialogLineNodeStyle.h>
#include <EdGraphUtilities.h>

DialogStyleSet::DialogStyleSet()
{
    _StyleSet = MakeShareable(new FSlateStyleSet(TEXT("DialogGraphEditorStyle")));
    TSharedPtr<IPlugin> plugin = IPluginManager::Get().FindPlugin("DialogGraph");
    FString contentDir = plugin->GetContentDir();
    _StyleSet->SetContentRoot(contentDir);

    _PinFactory = MakeShareable(new FDialogPinFactory());
    FEdGraphUtilities::RegisterVisualPinFactory(_PinFactory);

    _ChoicePinFactory = MakeShareable(new FChoicePinFactory());
    FEdGraphUtilities::RegisterVisualPinFactory(_ChoicePinFactory);

    _LineNodeFactory = MakeShareable(new FDialogLineNodeFactory());
    FEdGraphUtilities::RegisterVisualNodeFactory(_LineNodeFactory);
}

DialogStyleSet::~DialogStyleSet()
{
    FEdGraphUtilities::UnregisterVisualPinFactory(_PinFactory);
    FEdGraphUtilities::UnregisterVisualPinFactory(_ChoicePinFactory);
    FEdGraphUtilities::UnregisterVisualNodeFactory(_LineNodeFactory);
    _PinFactory = nullptr;
    _ChoicePinFactory = nullptr;
    _LineNodeFactory = nullptr;
}
