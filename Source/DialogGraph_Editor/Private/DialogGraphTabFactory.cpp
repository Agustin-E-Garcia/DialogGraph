#include <DialogGraphTabFactory.h>
#include <DialogAssetEditor.h>

FDialogGraphTabFactory::FDialogGraphTabFactory(TSharedPtr<FDialogAssetEditor> editor) : FWorkflowTabFactory(FName("DialogGraphTab"), editor)
{
    _DialogAssetEditor = editor;
    TabLabel = FText::FromString("Dialog Graph");
}

TSharedRef<SWidget> FDialogGraphTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& info) const
{
    return SNew(STextBlock).Text(FText::FromString(TEXT("Test text for now")));
}

FText FDialogGraphTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& info) const
{
    return FText::FromString(TEXT("Graph view to build and visualize dialogs"));
}
