#pragma once

#include <CoreMinimal.h>
#include <WorkflowOrientedApp/WorkflowTabFactory.h>

class FDialogAssetInspectorTabFactory : public FWorkflowTabFactory
{
public:
    FDialogAssetInspectorTabFactory(TSharedPtr<class FDialogAssetEditor> editor);

    virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& info) const override;
    virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& info) const override;
    
private:
    TWeakPtr<class FDialogAssetEditor> _DialogAssetEditor;
};
