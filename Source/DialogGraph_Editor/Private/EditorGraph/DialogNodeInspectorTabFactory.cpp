#include <EditorGraph/DialogNodeInspectorTabFactory.h>
#include <DialogAssetEditor.h>
#include <Kismet2/BlueprintEditorUtils.h>
#include <Kismet2/KismetEditorUtilities.h>
#include <GraphEditor.h>
#include <PropertyEditorModule.h>
#include <IStructureDetailsView.h>
#include <DialogAsset.h>
#include <UObject/StructOnScope.h>

FDialogNodeInspectorTabFactory::FDialogNodeInspectorTabFactory(TSharedPtr<FDialogAssetEditor> editor) : FWorkflowTabFactory(FName("DialogNodeInspectorTab"), editor)
{
    _DialogAssetEditor = editor;
    TabLabel = FText::FromString("Inspector");
}

TSharedRef<SWidget> FDialogNodeInspectorTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& info) const
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

    FStructureDetailsViewArgs structureDetailsViewArgs;
    {
        structureDetailsViewArgs.bShowAssets = false;
        structureDetailsViewArgs.bShowClasses = false;
        structureDetailsViewArgs.bShowInterfaces = false;
        structureDetailsViewArgs.bShowObjects = false;
    }

    TSharedPtr<FStructOnScope> scopeStruct = MakeShareable(new FStructOnScope(FDialogNode::StaticStruct(), reinterpret_cast<uint8*>(editor->GetWorkingAsset()->GetNode(0))));
    TSharedPtr<IStructureDetailsView> detailsView = propertyEditorModule.CreateStructureDetailView(detailsViewArgs, structureDetailsViewArgs, scopeStruct);

    return SNew(SVerticalBox)
                + SVerticalBox::Slot()
                .FillHeight(1.0f)
                .HAlign(HAlign_Fill)
                [
                    detailsView->GetWidget().ToSharedRef()
                ];

}

FText FDialogNodeInspectorTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& info) const
{
    return FText::FromString(TEXT("Inspector to visualize dialog asset data"));
}
