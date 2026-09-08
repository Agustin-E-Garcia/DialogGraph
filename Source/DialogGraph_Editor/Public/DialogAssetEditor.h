#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/UICommandList.h"
#include "Templates/SharedPointer.h"
#include "Templates/SubclassOf.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"
#include "EditorUndoClient.h"
#include "DialogAssetGraph.h"

class UDialogAsset;
class SGraphEditor;
class UEdGraphPin;
class UDialogGraphNode_Base;
struct FEdGraphEditAction;

class FDialogAssetEditor : public FWorkflowCentricApplication, public FEditorUndoClient
{
public:
    FDialogAssetEditor();
    virtual ~FDialogAssetEditor();

    virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& tabManager) override;

    void InitDialogAssetGraph(const EToolkitMode::Type mode, const TSharedPtr<IToolkitHost>& initToolkitHost, UObject* inObject);

    //~ Begin IToolkit interface
    virtual FName GetToolkitFName() const override { return FName(TEXT("DialogAssetEditor")); }
    virtual FText GetBaseToolkitName() const override { return FText::FromString("DialogAssetEditor"); }
    virtual FString GetWorldCentricTabPrefix() const override { return TEXT("Dialog Asset Editor"); }
    virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor(0.2, 0.2, 0.5, 0.8); };
    //~ End IToolkit interface

    //~ Begin FEditorUndoClient interface
    //~ End FEditorUndoClient interface

    /** Register the SGraphEditor object */
    void SetGraphEditor(TSharedPtr<class SGraphEditor> GraphEditor) { GraphEditorPtr = GraphEditor; }

    /** Get the dialog asset we're editing (if any) */
    UDialogAsset* GetDialogAsset() { return DialogAsset; }

    void RegisterToolbarTab(const TSharedRef<class FTabManager>& TabManager);

    /** Restores the dialog graph we were editing or creates a new one if none is available */
    void RestoreDialogAsset();

    /** Save the graph state for later editing */
    void SaveEditedObjectState();

    /** Get the editor commands to apply to this graph */
    TSharedPtr<FUICommandList> GetGraphEditorCommands();

protected:
    /** Called when "Save" is clicked for this asset */
    virtual void SaveAsset_Execute() override;

    TSubclassOf<UDialogAssetGraph> GraphClass;
    FName GraphName;

    TWeakPtr<class SGraphEditor> GraphEditorPtr;

    TSharedPtr<FUICommandList> GraphEditorCommands;

private:
    void CreateCommandList();

    void SelectAllNodes();
    bool CanSelectAllNodes();

    void DeleteSelectedNodes();
    bool CanDeleteSelectedNodes();

    void CopySelectedNodes();
    bool CanCopySelectedNodes();

    void CutSelectedNodes();
    bool CanCutSelectedNodes();

    void PasteSelectedNodes();
    bool CanPasteSelectedNodes();

    void DuplicateSelectedNodes();
    bool CanDuplicateSelectedNodes();

    UPROPERTY()
    UDialogAsset* DialogAsset = nullptr;

public:
    static const FName DialogGraphMode;
};
