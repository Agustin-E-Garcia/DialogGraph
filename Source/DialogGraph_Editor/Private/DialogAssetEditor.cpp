#include <DialogAssetEditor.h>
#include <EditorGraph/DialogGraphEditMode.h>
#include <EditorGraph/DialogGraphSchema.h>
#include <DialogAsset.h>
#include <Kismet2/BlueprintEditorUtils.h>

void FDialogAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& tabManager)
{
    FWorkflowCentricApplication::RegisterTabSpawners(tabManager);
}

void FDialogAssetEditor::InitEditor(const EToolkitMode::Type mode, const TSharedPtr<IToolkitHost>& initToolkitHost, UObject* inObject)
{
    _WorkingAsset = Cast<UDialogAsset>(inObject);
    _WorkingGraph = FBlueprintEditorUtils::CreateNewGraph(_WorkingAsset, *_WorkingAsset->GetName(), UEdGraph::StaticClass(), UDialogGraphSchema::StaticClass());

    InitAssetEditor(mode, initToolkitHost, TEXT("DialogAssetEditor"), FTabManager::FLayout::NullLayout, true, true, inObject);

    AddApplicationMode(TEXT("DialogGraphEditMode"), MakeShareable(new FDialogGraphEditMode(SharedThis(this))));
    SetCurrentMode(TEXT("DialogGraphEditMode"));
}
