#include <EditorGraph/DialogGraphEditMode.h>
#include <EditorGraph/DialogGraphTabFactory.h>
#include <EditorGraph/DialogAssetInspectorTabFactory.h>
#include <DialogAssetEditor.h>

FDialogGraphEditMode::FDialogGraphEditMode(TSharedPtr<FDialogAssetEditor> editor) : FApplicationMode(FName(TEXT("DialogGraphEditMode")))
{
    _DialogAssetEditor = editor;
    _AllowedTabs.RegisterFactory(MakeShareable(new FDialogGraphTabFactory(editor)));
    _AllowedTabs.RegisterFactory(MakeShareable(new FDialogAssetInspectorTabFactory(editor)));

    TabLayout = FTabManager::NewLayout("DialogGraphEditMode_v1.2")
        ->AddArea
        (
            FTabManager::NewPrimaryArea()
                ->SetOrientation(Orient_Vertical)
                ->Split
                (
                    FTabManager::NewSplitter()
                        ->SetOrientation(Orient_Horizontal)
                        ->Split
                        (
                            FTabManager::NewStack()
                                ->SetSizeCoefficient(0.75)
                                ->AddTab(FName(TEXT("DialogGraphTab")), ETabState::OpenedTab)
                        )
                        ->Split
                        (
                            FTabManager::NewStack()
                                ->SetSizeCoefficient(0.25)
                                ->AddTab(FName(TEXT("DialogAssetInspectorTab")), ETabState::OpenedTab)
                        )
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
