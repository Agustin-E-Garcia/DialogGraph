#pragma once

#include <CoreMinimal.h>
#include <WorkflowOrientedApp/WorkflowCentricApplication.h>

class FDialogAssetEditor : public FWorkflowCentricApplication
{
public:
    virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& tabManager) override;
    void InitEditor(const EToolkitMode::Type mode, const TSharedPtr<IToolkitHost>& initToolkitHost, UObject* inObject);

public:
    virtual FName GetToolkitFName() const override { return FName(TEXT("DialogAssetEditor")); }
    virtual FText GetBaseToolkitName() const override { return FText::FromString("DialogAssetEditor"); }
    virtual FString GetWorldCentricTabPrefix() const override { return TEXT("Dialog Asset Editor"); }
    virtual FLinearColor GetDefaultTabColor() const override { return FLinearColor(0.2, 0.2, 0.5, 0.8); }
    virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor(0.2, 0.2, 0.5, 0.8); };
    virtual FString GetDocumentationLink() const override { return TEXT("https://github.com/Agustin-E-Garcia/DialogGraph"); }
    virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>& toolkit) override {};
    virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>& toolkit) override {};
};
