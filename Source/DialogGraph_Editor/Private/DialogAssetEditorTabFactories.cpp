#include "DialogAssetEditorTabFactories.h"
#include "DialogAssetEditorTabs.h"
#include "DialogAsset.h"
#include "PropertyEditorModule.h"

#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SNullWidget.h"
#include "DialogAssetEditor.h"
#include "GraphEditor.h"

///////////////////////////////////////
// Dialog Asset Graph Tab Factory
//////////////////////////////////////

FDialogAssetGraphTabFactory::FDialogAssetGraphTabFactory(TSharedPtr<FDialogAssetEditor> InDialogAssetEditor) : FWorkflowTabFactory(FDialogAssetEditorTabs::GraphEditorID, InDialogAssetEditor)
{
    DialogAssetEditorPtr = InDialogAssetEditor;
    TabLabel = FText::FromString("Dialog Graph");

    SAssignNew(DialogGraphEditorPtr, SGraphEditor)
                .AdditionalCommands(InDialogAssetEditor->GetGraphEditorCommands())
                .IsEditable(true)
                .GraphToEdit(InDialogAssetEditor->GetDialogAsset()->DialogGraph);

    InDialogAssetEditor->SetGraphEditor(DialogGraphEditorPtr);
}

TSharedRef<SWidget> FDialogAssetGraphTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& info) const
{
    TSharedPtr<FDialogAssetEditor> editor = DialogAssetEditorPtr.Pin();
    if (!editor.IsValid()) return SNullWidget::NullWidget;

    return SNew(SVerticalBox)
            +SVerticalBox::Slot()
            .FillHeight(1.0f)
            .HAlign(HAlign_Fill)
            [
                DialogGraphEditorPtr.ToSharedRef()
            ];
}

FText FDialogAssetGraphTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& info) const
{
    return FText::FromString(TEXT("Graph view to build and visualize dialogs"));
}

///////////////////////////////////////
// Dialog Asset Inspector Tab Factory
//////////////////////////////////////

FDialogAssetInspectorTabFactory::FDialogAssetInspectorTabFactory(TSharedPtr<FDialogAssetEditor> InDialogAssetEditorPtr) : FWorkflowTabFactory(FDialogAssetEditorTabs::AssetInspectorID, InDialogAssetEditorPtr)
{
    DialogAssetEditorPtr = InDialogAssetEditorPtr;
    TabLabel = FText::FromString("Inspector");
}

TSharedRef<SWidget> FDialogAssetInspectorTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& info) const
{
    TSharedPtr<FDialogAssetEditor> editor = DialogAssetEditorPtr.Pin();
    if (!editor.IsValid())
    {
        return SNullWidget::NullWidget;
    }

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
    detailsView->SetObject(Cast<UObject>(editor->GetDialogAsset()));

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
