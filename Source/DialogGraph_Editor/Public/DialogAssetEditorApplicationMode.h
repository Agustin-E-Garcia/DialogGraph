#pragma once

#include <CoreMinimal.h>
#include <WorkflowOrientedApp/ApplicationMode.h>
#include <WorkflowOrientedApp/WorkflowTabManager.h>

class FDialogAssetEditorApplicationMode : public FApplicationMode
{
public:
    FDialogAssetEditorApplicationMode(TSharedPtr<class FDialogAssetEditor> editor);

    virtual void RegisterTabFactories(TSharedPtr<class FTabManager> inTabManager) override;
    virtual void PreDeactivateMode() override;
    virtual void PostActivateMode() override;

protected:
    TWeakPtr<class FDialogAssetEditor> DialogAssetEditor;

    // Set of spawnable tabs in behaviour tree editing mode
    FWorkflowAllowedTabSet DialogAssetEditorTabFactories;
};
