#include <EditorGraph/DialogAssetInspectorTabFactory.h>
#include <DialogAssetEditor.h>
#include <Kismet2/BlueprintEditorUtils.h>
#include <Kismet2/KismetEditorUtilities.h>
#include <GraphEditor.h>
#include <PropertyEditorModule.h>

FDialogAssetInspectorTabFactory::FDialogAssetInspectorTabFactory(TSharedPtr<FDialogAssetEditor> editor) : FWorkflowTabFactory(FName("DialogAssetInspectorTab"), editor)
{
    _DialogAssetEditor = editor;
    TabLabel = FText::FromString("Inspector");
}

TSharedRef<SWidget> FDialogAssetInspectorTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& info) const
{
    TSharedPtr<FDialogAssetEditor> editor = _DialogAssetEditor.Pin();
    FPropertyEditorModule& propertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

    FDetailsViewArgs detailsViewArgs;
    {
        detailsViewArgs.bAllowSearch = false;
        detailsViewArgs.bHideSelectionTip = true;
        detailsViewArgs.bLockable = false;
        detailsViewArgs.bSearchInitialKeyFocus = true;
        detailsViewArgs.bUpdatesFromSelection = false;
        detailsViewArgs.NotifyHook = nullptr;
        detailsViewArgs.bShowOptions = true;
        detailsViewArgs.bShowModifiedPropertiesOption = false;
        detailsViewArgs.bShowScrollBar = false;
    }

    TSharedPtr<IDetailsView> detailsView = propertyEditorModule.CreateDetailView(detailsViewArgs);
    detailsView->SetObject(Cast<UObject>(editor->GetWorkingAsset()));

    return SNew(SVerticalBox)
                + SVerticalBox::Slot()
                .FillHeight(1.0f)
                .HAlign(HAlign_Fill)
                [
                    detailsView.ToSharedRef()
                ];
}

FText FDialogAssetInspectorTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& info) const
{
    return FText::FromString(TEXT("Inspector to visualize dialog asset data"));
}
