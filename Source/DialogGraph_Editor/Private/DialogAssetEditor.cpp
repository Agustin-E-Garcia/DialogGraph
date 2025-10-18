#include <DialogAssetEditor.h>
#include <DialogGraphEditMode.h>

void FDialogAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& tabManager)
{
    FWorkflowCentricApplication::RegisterTabSpawners(tabManager);
}

void FDialogAssetEditor::InitEditor(const EToolkitMode::Type mode, const TSharedPtr<IToolkitHost>& initToolkitHost, UObject* inObject)
{
    InitAssetEditor(mode, initToolkitHost, TEXT("DialogAssetEditor"), FTabManager::FLayout::NullLayout, true, true, inObject);

    AddApplicationMode(TEXT("DialogGraphEditMode"), MakeShareable(new FDialogGraphEditMode(SharedThis(this))));

    SetCurrentMode(TEXT("DialogGraphEditMode"));
}
