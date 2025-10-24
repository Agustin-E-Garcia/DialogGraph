#include <EditorGraph/DialogGraphTabFactory.h>
#include <DialogAssetEditor.h>
#include <Kismet2/BlueprintEditorUtils.h>
#include <Kismet2/KismetEditorUtilities.h>
#include <GraphEditor.h>

FDialogGraphTabFactory::FDialogGraphTabFactory(TSharedPtr<FDialogAssetEditor> editor) : FWorkflowTabFactory(FName("DialogGraphTab"), editor)
{
    _DialogAssetEditor = editor;
    TabLabel = FText::FromString("Dialog Graph");
}

TSharedRef<SWidget> FDialogGraphTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& info) const
{
    TSharedPtr<FDialogAssetEditor> editor = _DialogAssetEditor.Pin();

    return SNew(SVerticalBox) + SVerticalBox::Slot()
                                .FillHeight(1.0f)
                                .HAlign(HAlign_Fill)
                                [
                                    SNew(SGraphEditor)
                                            .IsEditable(true)
                                            .GraphToEdit(editor->GetWorkingGraph())
                                ];
}

FText FDialogGraphTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& info) const
{
    return FText::FromString(TEXT("Graph view to build and visualize dialogs"));
}
