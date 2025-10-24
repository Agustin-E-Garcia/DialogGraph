#pragma once

#include <CoreMinimal.h>
#include <WorkflowOrientedApp/ApplicationMode.h>
#include <WorkflowOrientedApp/WorkflowTabManager.h>

class FDialogGraphEditMode : public FApplicationMode
{
public:
    FDialogGraphEditMode(TSharedPtr<class FDialogAssetEditor> editor);

    virtual void RegisterTabFactories(TSharedPtr<class FTabManager> inTabManager) override;
    virtual void PreDeactivateMode() override;
    virtual void PostActivateMode() override;

private:
    TWeakPtr<class FDialogAssetEditor> _DialogAssetEditor;
    FWorkflowAllowedTabSet _AllowedTabs;
};
