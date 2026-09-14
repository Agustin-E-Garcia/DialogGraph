#pragma once

#include "CoreMinimal.h"
#include "DetailsViewArgs.h"
#include "Framework/Commands/UICommandList.h"
#include "Templates/SharedPointer.h"
#include "Templates/SubclassOf.h"
#include "UObject/UnrealType.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"
#include "EditorUndoClient.h"
#include "DialogAssetGraph.h"
#include "Misc/NotifyHook.h"

class UDialogAsset;
class SGraphEditor;
class UEdGraphPin;
class UDialogGraphNode_Base;
struct FEdGraphEditAction;

class FDialogAssetEditor : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook
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

    //~ Begin FNotifyHook interface
    virtual void NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FProperty* PropertyThatChanged) override;
    //~ End FNotifyHook interface

    bool IsPropertyEditable() const;
    void OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent);

    /** Register the SGraphEditor object */
    void SetGraphEditor(TSharedPtr<class SGraphEditor> GraphEditor) { GraphEditorPtr = GraphEditor; }

    /** Get the dialog asset we're editing (if any) */
    UDialogAsset* GetDialogAsset() { return DialogAsset; }

    void RegisterToolbarTab(const TSharedRef<class FTabManager>& TabManager);

    /** Restores the dialog graph we were editing or creates a new one if none is available */
    void RestoreDialogAsset();

    /** Save the graph state for later editing */
    void SaveEditedObjectState();

    TSharedRef<SWidget> SpawnProperties();

    /** Get the editor commands to apply to this graph */
    TSharedPtr<FUICommandList> GetGraphEditorCommands();

    /** Called when the selection changes in the GraphEditor */
    void OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection);

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

    /** Creates all internal widgets for the tabs to point at */
    void CreateInternalWidgets();

    /** The Dialog Asset being edited */
    UDialogAsset* DialogAsset = nullptr;

    /** Property Editor */
    TSharedPtr<class IDetailsView> DetailsView;

public:
    static const FName DialogGraphMode;
};
