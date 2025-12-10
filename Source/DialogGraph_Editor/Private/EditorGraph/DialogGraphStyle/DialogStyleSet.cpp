#include <EditorGraph/DialogGraphStyle/DialogStyleSet.h>
#include <Interfaces/IPluginManager.h>
#include <EditorGraph/DialogGraphStyle/DialogPinStyle.h>
#include <EditorGraph/DialogGraphStyle/DialogLineNodeStyle.h>
#include <EdGraphUtilities.h>
#include <EditorGraph/DialogGraphStyle/DialogTransitionNodeStyle.h>

DialogStyleSet::DialogStyleSet()
{
    _StyleSet = MakeShareable(new FSlateStyleSet(TEXT("DialogGraphEditorStyle")));
    TSharedPtr<IPlugin> plugin = IPluginManager::Get().FindPlugin("DialogGraph");
    FString contentDir = plugin->GetContentDir();
    _StyleSet->SetContentRoot(contentDir);

    _PinFactory = MakeShareable(new FDialogPinFactory());
    FEdGraphUtilities::RegisterVisualPinFactory(_PinFactory);

    _NodeLineFactory = MakeShareable(new FDialogLineNodeFactory());
    FEdGraphUtilities::RegisterVisualNodeFactory(_NodeLineFactory);

    _NodeTransitionFactory = MakeShareable(new FDialogTransitionNodeFactory());
    FEdGraphUtilities::RegisterVisualNodeFactory(_NodeTransitionFactory);
}

DialogStyleSet::~DialogStyleSet()
{
    FEdGraphUtilities::UnregisterVisualPinFactory(_PinFactory);
    FEdGraphUtilities::UnregisterVisualNodeFactory(_NodeLineFactory);
    _PinFactory = nullptr;
    _NodeLineFactory = nullptr;
    _NodeTransitionFactory = nullptr;
}
