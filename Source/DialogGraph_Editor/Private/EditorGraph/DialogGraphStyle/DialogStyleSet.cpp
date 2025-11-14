#include <EditorGraph/DialogGraphStyle/DialogStyleSet.h>
#include <Interfaces/IPluginManager.h>
#include <EditorGraph/DialogGraphStyle/DialogPinStyle.h>
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

    _NodeFactory = MakeShareable(new FDialogLineNodeFactory());
    FEdGraphUtilities::RegisterVisualNodeFactory(_NodeFactory);
}

DialogStyleSet::~DialogStyleSet()
{
    FEdGraphUtilities::UnregisterVisualPinFactory(_PinFactory);
    FEdGraphUtilities::UnregisterVisualNodeFactory(_NodeFactory);
    _PinFactory = nullptr;
    _NodeFactory = nullptr;
}
