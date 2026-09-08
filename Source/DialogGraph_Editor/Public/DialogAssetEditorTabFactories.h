#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

struct FDialogAssetGraphTabFactory : public FWorkflowTabFactory
{
public:
    FDialogAssetGraphTabFactory(TSharedPtr<class FDialogAssetEditor> InDialogAssetEditorPtr);

protected:
    virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& info) const override;
    virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& info) const override;

private:
    void OnFocused(const TSharedRef<class SGraphEditor>& GraphEditor) const;

    TSharedPtr<class SGraphEditor> DialogGraphEditorPtr;
    TWeakPtr<class FDialogAssetEditor> DialogAssetEditorPtr;
};


struct FDialogAssetInspectorTabFactory : public FWorkflowTabFactory
{
public:
    FDialogAssetInspectorTabFactory(TSharedPtr<class FDialogAssetEditor> InDialogAssetEditor);

    virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
    virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:
    TWeakPtr<class FDialogAssetEditor> DialogAssetEditorPtr;
};
