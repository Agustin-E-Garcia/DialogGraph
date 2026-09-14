#pragma once

#include "CoreMinimal.h"
#include "Templates/SharedPointer.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

struct FDialogAssetGraphTabFactory : public FWorkflowTabFactory
{
public:
    FDialogAssetGraphTabFactory(TSharedPtr<class FDialogAssetEditor> InDialogAssetEditorPtr);

    // Must be called after this factory has been wrapped in a shared pointer (BindSP below needs AsShared() to work).
    void Initialize();

protected:
    virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& info) const override;
    virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& info) const override;

private:
    void OnSelectionChanged(const TSet<class UObject*>& NewSelection);

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

struct FDialogGraphDetailsTabFactory : public FWorkflowTabFactory
{
public:
    FDialogGraphDetailsTabFactory(TSharedPtr<class FDialogAssetEditor> InDialogAssetEditor);

    virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
    virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:
    TWeakPtr<class FDialogAssetEditor> DialogAssetEditorPtr;
};
