#include <DialogGraphEditMode.h>
#include <DialogAssetEditor.h>
#include <DialogGraphTabFactory.h>

FDialogGraphEditMode::FDialogGraphEditMode(TSharedPtr<FDialogAssetEditor> editor) : FApplicationMode(FName(TEXT("DialogGraphEditMode")))
{
    _DialogAssetEditor = editor;
    _AllowedTabs.RegisterFactory(MakeShareable(new FDialogGraphTabFactory(editor)));

    TabLayout = FTabManager::NewLayout("DialogGraphEditMode_v1")->AddArea
        (
            FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)->Split
            (
                FTabManager::NewStack()->AddTab(FName(TEXT("DialogGraphTab")), ETabState::OpenedTab)
            )
        );
}

void FDialogGraphEditMode::RegisterTabFactories(TSharedPtr<FTabManager> inTabManager) 
{
    TSharedPtr<FDialogAssetEditor> editor = _DialogAssetEditor.Pin();
    editor->PushTabFactories(_AllowedTabs);
    FApplicationMode::RegisterTabFactories(inTabManager);
}

void FDialogGraphEditMode::PreDeactivateMode() { FApplicationMode::PreDeactivateMode(); }
void FDialogGraphEditMode::PostActivateMode() { FApplicationMode::PostActivateMode(); }
