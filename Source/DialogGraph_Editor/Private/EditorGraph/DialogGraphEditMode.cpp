#include <EditorGraph/DialogGraphEditMode.h>
#include <EditorGraph/DialogGraphTabFactory.h>
#include <EditorGraph/DialogAssetInspectorTabFactory.h>
#include <EditorGraph/DialogNodeInspectorTabFactory.h>
#include <DialogAssetEditor.h>

FDialogGraphEditMode::FDialogGraphEditMode(TSharedPtr<FDialogAssetEditor> editor) : FApplicationMode(FName(TEXT("DialogGraphEditMode")))
{
    _DialogAssetEditor = editor;
    _AllowedTabs.RegisterFactory(MakeShareable(new FDialogGraphTabFactory(editor)));
    _AllowedTabs.RegisterFactory(MakeShareable(new FDialogAssetInspectorTabFactory(editor)));
    _AllowedTabs.RegisterFactory(MakeShareable(new FDialogNodeInspectorTabFactory(editor)));

    TabLayout = FTabManager::NewLayout("DialogGraphEditMode_v1.3")
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
                                ->SetSizeCoefficient(0.50)
                                ->AddTab(FName(TEXT("DialogGraphTab")), ETabState::OpenedTab)
                        )
                        ->Split
                        (
                            FTabManager::NewStack()
                                ->SetSizeCoefficient(0.25)
                                ->AddTab(FName(TEXT("DialogAssetInspectorTab")), ETabState::OpenedTab)
                        )
                        ->Split
                        (
                            FTabManager::NewStack()
                                ->SetSizeCoefficient(0.25)
                                ->AddTab(FName(TEXT("DialogNodeInspectorTab")), ETabState::OpenedTab)
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
