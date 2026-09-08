#include "DialogAssetEditorApplicationMode.h"
#include "DialogAssetEditor.h"
#include "DialogAssetEditorTabFactories.h"
#include "DialogAssetEditorTabs.h"

FDialogAssetEditorApplicationMode::FDialogAssetEditorApplicationMode(TSharedPtr<FDialogAssetEditor> InDialogAssetEditor) : FApplicationMode(FDialogAssetEditor::DialogGraphMode)
{
    DialogAssetEditor = InDialogAssetEditor;
    DialogAssetEditorTabFactories.RegisterFactory(MakeShareable(new FDialogAssetGraphTabFactory(InDialogAssetEditor)));
    DialogAssetEditorTabFactories.RegisterFactory(MakeShareable(new FDialogAssetInspectorTabFactory(InDialogAssetEditor)));

    TabLayout = FTabManager::NewLayout("Standalone_DialogAsset_Layout_v1.1")
    ->AddArea
    (
        FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
        ->Split
        (
            FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
            ->Split
            (
                FTabManager::NewStack()
                ->SetSizeCoefficient(0.7f)
                ->AddTab(FDialogAssetEditorTabs::GraphEditorID, ETabState::OpenedTab)
            )
            ->Split
            (
                FTabManager::NewStack()
                ->SetSizeCoefficient(0.3f)
                ->AddTab(FDialogAssetEditorTabs::AssetInspectorID, ETabState::OpenedTab)
            )
        )
    );
}

void FDialogAssetEditorApplicationMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
    check(DialogAssetEditor.IsValid());
    TSharedPtr<FDialogAssetEditor> DialogAssetEditorPtr = DialogAssetEditor.Pin();

    DialogAssetEditorPtr->RegisterToolbarTab(InTabManager.ToSharedRef());

    DialogAssetEditorPtr->PushTabFactories(DialogAssetEditorTabFactories);
}

void FDialogAssetEditorApplicationMode::PreDeactivateMode() 
{
    FApplicationMode::PreDeactivateMode();

    check(DialogAssetEditor.IsValid());
    TSharedPtr<FDialogAssetEditor> DialogAssetEditorPtr = DialogAssetEditor.Pin();

    DialogAssetEditorPtr->SaveEditedObjectState();
}

void FDialogAssetEditorApplicationMode::PostActivateMode() 
{
    FApplicationMode::PostActivateMode();
}
