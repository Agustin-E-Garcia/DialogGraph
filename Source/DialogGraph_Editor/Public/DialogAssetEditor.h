#pragma once

#include "Containers/Set.h"
#include <CoreMinimal.h>
#include <WorkflowOrientedApp/WorkflowCentricApplication.h>

class UDialogAsset;
class UEdGraph;
class UEdGraphPin;
class UDialogGraphNode_Base;
struct FEdGraphEditAction;

class FDialogAssetEditor : public FWorkflowCentricApplication
{
public:
    virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& tabManager) override;
    void InitEditor(const EToolkitMode::Type mode, const TSharedPtr<IToolkitHost>& initToolkitHost, UObject* inObject);

    void InitializeGraphFromAsset();
    void UpdateAssetFromGraph();

    UDialogAsset* GetWorkingAsset() { return _WorkingAsset; }
    UEdGraph* GetWorkingGraph() { return _WorkingGraph; }

public: // IToolkit interface
    virtual FName GetToolkitFName() const override { return FName(TEXT("DialogAssetEditor")); }
    virtual FText GetBaseToolkitName() const override { return FText::FromString("DialogAssetEditor"); }
    virtual FString GetWorldCentricTabPrefix() const override { return TEXT("Dialog Asset Editor"); }
    virtual FLinearColor GetDefaultTabColor() const override { return FLinearColor(0.2, 0.2, 0.5, 0.8); }
    virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor(0.2, 0.2, 0.5, 0.8); };
    virtual FString GetDocumentationLink() const override { return TEXT("https://github.com/Agustin-E-Garcia/DialogGraph"); }
    virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>& toolkit) override {};
    virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>& toolkit) override {};

    virtual void OnClose() override;
    void OnGraphChanged(const FEdGraphEditAction& editAction);

private:
    UPROPERTY()
    UDialogAsset* _WorkingAsset = nullptr;
    UEdGraph* _WorkingGraph = nullptr;

    FDelegateHandle _OnGraphChangedHandle;

    TSet<int> _RemovedNodes;

    void FillToolbar(FToolBarBuilder& Builder);
    void CreateGraphNode(int runtimeNodeID, UEdGraphPin* fromPin);
    int CreateRuntimeNode(UDialogGraphNode_Base* graphNode);
};
