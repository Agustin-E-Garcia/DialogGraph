#include "DialogAssetEditorApplicationMode.h"
#include "DialogAssetEditor.h"
#include "DialogAssetEditorTabFactories.h"
#include "DialogAssetEditorTabs.h"
#include "Templates/SharedPointer.h"

FDialogAssetEditorApplicationMode::FDialogAssetEditorApplicationMode(TSharedPtr<FDialogAssetEditor> InDialogAssetEditor) : FApplicationMode(FDialogAssetEditor::DialogGraphMode)
{
    DialogAssetEditor = InDialogAssetEditor;

    TSharedRef<FDialogAssetGraphTabFactory> GraphTabFactory = MakeShareable(new FDialogAssetGraphTabFactory(InDialogAssetEditor));
    GraphTabFactory->Initialize();
    DialogAssetEditorTabFactories.RegisterFactory(GraphTabFactory);
    DialogAssetEditorTabFactories.RegisterFactory(MakeShareable(new FDialogGraphDetailsTabFactory(InDialogAssetEditor)));

    TabLayout = FTabManager::NewLayout("Standalone_DialogAsset_Layout_v1.2")
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
                ->AddTab(FDialogAssetEditorTabs::GraphDetailsID, ETabState::OpenedTab)
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
