#include "DialogAssetEditor.h"
#include "DetailsViewArgs.h"
#include "DialogAssetGraph.h"
#include "DialogGraph_Editor.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphSchema.h"
#include "EdGraphUtilities.h"
#include "Editor/EditorEngine.h"
#include "Engine/Engine.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Commands/UIAction.h"
#include "Framework/Commands/UICommandList.h"
#include "Framework/Docking/TabManager.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "GraphEditor.h"
#include "HAL/Platform.h"
#include "Misc/Guid.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorDelegates.h"
#include "PropertyEditorModule.h"
#include "ScopedTransaction.h"
#include "Templates/SharedPointer.h"
#include "Templates/SubclassOf.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Types/SlateEnums.h"
#include "DialogAssetEditorApplicationMode.h"
#include "DialogAsset.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Framework/Commands/GenericCommands.h"
#include "UObject/UnrealType.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SBoxPanel.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"
#include "HAL/PlatformApplicationMisc.h"

const FName FDialogAssetEditor::DialogGraphMode(TEXT("DialogGraph"));

FDialogAssetEditor::FDialogAssetEditor()
{
    DialogAsset = nullptr;

    GraphClass = UDialogAssetGraph::StaticClass();
    GraphName = "Dialog Asset";

    UEditorEngine* Editor = (UEditorEngine*)GEngine;
    if(Editor) Editor->RegisterForUndo(this);
}

FDialogAssetEditor::~FDialogAssetEditor()
{
}

FName FDialogAssetEditor::GetToolkitFName() const
{
    return FName(TEXT("DialogAssetEditor"));
}

FText FDialogAssetEditor::GetBaseToolkitName() const
{
    return FText::FromString("DialogAssetEditor");
}

FString FDialogAssetEditor::GetWorldCentricTabPrefix() const
{
    return TEXT("Dialog Asset Editor");
}

FLinearColor FDialogAssetEditor::GetWorldCentricTabColorScale() const
{
    return FLinearColor(0.2, 0.2, 0.5, 0.8);
};

void FDialogAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager)
{
    FWorkflowCentricApplication::RegisterTabSpawners(TabManager);
}

void FDialogAssetEditor::InitDialogAssetGraph(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* InObject)
{
    UDialogAsset* AssetToEdit = Cast<UDialogAsset>(InObject);

    if(AssetToEdit != nullptr)
    {
        DialogAsset = Cast<UDialogAsset>(InObject);
    }

    const bool bCreateDefaultStandaloneMenu = true;
    const bool bCreateDefaultToolbar = true;
    InitAssetEditor(Mode, InitToolkitHost, FDialogGraph_EditorModule::DialogAssetEditorAppIdentifier, FTabManager::FLayout::NullLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, InObject);

    FDialogGraph_EditorModule& DialogGraphEditorModule = FModuleManager::LoadModuleChecked<FDialogGraph_EditorModule>("DialogGraph_Editor");
    AddToolbarExtender(DialogGraphEditorModule.GetToolBarExtensibilityManager()->GetAllExtenders(GetToolkitCommands(), GetEditingObjects()));

    CreateCommandList();
    CreateInternalWidgets();
    RestoreDialogAsset();

    AddApplicationMode(DialogGraphMode, MakeShareable(new FDialogAssetEditorApplicationMode(SharedThis(this))));
    SetCurrentMode(DialogGraphMode);
}

void FDialogAssetEditor::RegisterToolbarTab(const TSharedRef<class FTabManager>& InTabManager)
{
    FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}

void FDialogAssetEditor::RestoreDialogAsset()
{
    UDialogAssetGraph* MyGraph = Cast<UDialogAssetGraph>(DialogAsset->DialogGraph);
    const bool bNewGraph = MyGraph == NULL;
    if (MyGraph == NULL)
    {
        const TSubclassOf<UEdGraphSchema> SchemaClass = GetDefault<UDialogAssetGraph>(GraphClass)->Schema;
        check(SchemaClass);
        DialogAsset->DialogGraph = FBlueprintEditorUtils::CreateNewGraph(DialogAsset, GraphName, GraphClass, SchemaClass);
        MyGraph = Cast<UDialogAssetGraph>(DialogAsset->DialogGraph);

        const UEdGraphSchema* Schema = MyGraph->GetSchema();
        Schema->CreateDefaultNodesForGraph(*MyGraph);

        MyGraph->OnCreated();
    }
    else
    {
        //MyGraph->OnLoaded();
    }
}

void FDialogAssetEditor::SaveEditedObjectState()
{
}

TSharedPtr<FUICommandList> FDialogAssetEditor::GetGraphEditorCommands()
{
    return GraphEditorCommands;
}

void FDialogAssetEditor::CreateCommandList()
{
    if(!GraphEditorCommands.IsValid())
    {
        GraphEditorCommands = MakeShareable(new FUICommandList);

        GraphEditorCommands->MapAction(FGenericCommands::Get().SelectAll,
            FExecuteAction::CreateSP(this, &FDialogAssetEditor::SelectAllNodes),
            FCanExecuteAction::CreateSP(this, &FDialogAssetEditor::CanSelectAllNodes)
            );

        GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
            FExecuteAction::CreateSP(this, &FDialogAssetEditor::DeleteSelectedNodes),
            FCanExecuteAction::CreateSP(this, &FDialogAssetEditor::CanDeleteSelectedNodes)
            );

        GraphEditorCommands->MapAction(FGenericCommands::Get().Copy,
            FExecuteAction::CreateSP(this, &FDialogAssetEditor::CopySelectedNodes),
            FCanExecuteAction::CreateSP(this, &FDialogAssetEditor::CanCopySelectedNodes)
            );

        GraphEditorCommands->MapAction(FGenericCommands::Get().Cut,
            FExecuteAction::CreateSP(this, &FDialogAssetEditor::CutSelectedNodes),
            FCanExecuteAction::CreateSP(this, &FDialogAssetEditor::CanCutSelectedNodes)
            );

        GraphEditorCommands->MapAction(FGenericCommands::Get().Paste,
            FExecuteAction::CreateSP(this, &FDialogAssetEditor::PasteSelectedNodes),
            FCanExecuteAction::CreateSP(this, &FDialogAssetEditor::CanPasteSelectedNodes)
            );

        GraphEditorCommands->MapAction(FGenericCommands::Get().Duplicate,
            FExecuteAction::CreateSP(this, &FDialogAssetEditor::DuplicateSelectedNodes),
            FCanExecuteAction::CreateSP(this, &FDialogAssetEditor::CanDuplicateSelectedNodes)
            );
    }
}

void FDialogAssetEditor::SelectAllNodes()
{
    if (TSharedPtr<SGraphEditor> CurrentGraphEditor = GraphEditorPtr.Pin())
    {
        CurrentGraphEditor->SelectAllNodes();
    }
}
bool FDialogAssetEditor::CanSelectAllNodes() { return true; }

void FDialogAssetEditor::DeleteSelectedNodes()
{
    TSharedPtr<SGraphEditor> CurrentGraphEditor = GraphEditorPtr.Pin();
    if(!CurrentGraphEditor.IsValid()) return;

    const FScopedTransaction Transaction(FGenericCommands::Get().Delete->GetDescription());
    CurrentGraphEditor->GetCurrentGraph()->Modify();

    const FGraphPanelSelectionSet SelectedNodes = CurrentGraphEditor->GetSelectedNodes();

    for(FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
    {
        if(UEdGraphNode* Node = Cast<UEdGraphNode>(*NodeIt))
        {
            if(Node->CanUserDeleteNode())
            {
                Node->Modify();
                Node->DestroyNode();
            }
        }
    }
}

bool FDialogAssetEditor::CanDeleteSelectedNodes()
{
    // If any of the nodes can be deleted then we should allow deleting
    TSharedPtr<SGraphEditor> CurrentGraphEditor = GraphEditorPtr.Pin();
    if(!CurrentGraphEditor.IsValid()) return false;

    const FGraphPanelSelectionSet SelectedNodes = CurrentGraphEditor->GetSelectedNodes();
    for(FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
    {
        UEdGraphNode* Node = Cast<UEdGraphNode>(*NodeIt);
        if(Node && Node->CanUserDeleteNode()) return true;
    }

    return false;
}

void FDialogAssetEditor::CopySelectedNodes()
{
    TSharedPtr<SGraphEditor> CurrentGraphEditor = GraphEditorPtr.Pin();
    if(!CurrentGraphEditor.IsValid()) return;

    FGraphPanelSelectionSet SelectedNodes = CurrentGraphEditor->GetSelectedNodes();

    FString ExportedText;
    for(FGraphPanelSelectionSet::TIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
    {
        UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
        if(Node == nullptr || !Node->CanDuplicateNode())
        {
            SelectedIter.RemoveCurrent();
            continue;
        }

        Node->PrepareForCopying();
    }

    FEdGraphUtilities::ExportNodesToText(SelectedNodes, ExportedText);
    FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
}

bool FDialogAssetEditor::CanCopySelectedNodes() 
{
    // If any of the nodes can be duplicated then we should allow copying
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GraphEditorPtr.Pin();
    if(!CurrentGraphEditor.IsValid()) return false;

    const FGraphPanelSelectionSet SelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode())
		{
			return true;
		}
	}

	return false;
}

void FDialogAssetEditor::CutSelectedNodes() 
{
    CopySelectedNodes();
    DeleteSelectedNodes();
}

bool FDialogAssetEditor::CanCutSelectedNodes()
{ 
    return CanCopySelectedNodes() && CanDeleteSelectedNodes();
}

void FDialogAssetEditor::PasteSelectedNodes()
{
    TSharedPtr<SGraphEditor> CurrentGraphEditor = GraphEditorPtr.Pin();
    if(!CurrentGraphEditor) return;

    const FScopedTransaction Transaction(FGenericCommands::Get().Paste->GetDescription());
    UDialogAssetGraph* DialogGraph = Cast<UDialogAssetGraph>(CurrentGraphEditor->GetCurrentGraph());

    if(DialogGraph) DialogGraph->Modify();

    FString TextToImport;
    FPlatformApplicationMisc::ClipboardPaste(TextToImport);

    TSet<UEdGraphNode*> PastedNodes;
    FEdGraphUtilities::ImportNodesFromText(DialogGraph, TextToImport, PastedNodes);

    FVector2D AvgNodePosition(0.0f, 0.0f);
    int32 AvgCount = 0;

    for(TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
    {
        UEdGraphNode* EdNode = *It;
        if(EdNode)
        {
            AvgNodePosition.X += EdNode->NodePosX;
            AvgNodePosition.Y += EdNode->NodePosY;
            ++AvgCount;
        }
    }

    if(AvgCount > 0)
    {
        float InvNumNodes = 1.0f / float(AvgCount);
        AvgNodePosition.X *= InvNumNodes;
        AvgNodePosition.Y *= InvNumNodes;
    }

    TMap<FGuid, FGuid> NewToOldNodeMapping;
    FVector2f Location = CurrentGraphEditor->GetPasteLocation2f();
    for(TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
    {
        UEdGraphNode* PasteNode = *It;
        if(!PasteNode) continue;

        CurrentGraphEditor->SetNodeSelection(PasteNode, true);

        const FVector::FReal NodePosX = (PasteNode->NodePosX - AvgNodePosition.X) + Location.X;
        const FVector::FReal NodePosY = (PasteNode->NodePosY - AvgNodePosition.Y) + Location.Y;

        PasteNode->NodePosX = static_cast<int32>(NodePosX);
        PasteNode->NodePosY = static_cast<int32>(NodePosY);

        PasteNode->SnapToGrid(16);

        const FGuid OldGuid = PasteNode->NodeGuid;

        PasteNode->CreateNewGuid();

        const FGuid NewGuid = PasteNode->NodeGuid;

        NewToOldNodeMapping.Add(NewGuid, OldGuid);
    }

    // Update UI
    CurrentGraphEditor->NotifyGraphChanged();

    UObject* GraphOwner = DialogGraph->GetOuter();
    if (GraphOwner)
    {
        GraphOwner->PostEditChange();
        GraphOwner->MarkPackageDirty();
    }
}

bool FDialogAssetEditor::CanPasteSelectedNodes()
{
    TSharedPtr<SGraphEditor> CurrentGraphEditor = GraphEditorPtr.Pin();
    if (!CurrentGraphEditor.IsValid()) return false;

    FString ClipboardContent;
    FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);

    return FEdGraphUtilities::CanImportNodesFromText(CurrentGraphEditor->GetCurrentGraph(), ClipboardContent);
}

void FDialogAssetEditor::DuplicateSelectedNodes() 
{
    CopySelectedNodes();
    PasteSelectedNodes();
}

bool FDialogAssetEditor::CanDuplicateSelectedNodes() { return CanCopySelectedNodes(); }

void FDialogAssetEditor::SaveAsset_Execute()
{
    if(DialogAsset)
    {
        UDialogAssetGraph* DialogGraph = Cast<UDialogAssetGraph>(DialogAsset->DialogGraph);
        if(DialogGraph)
        {
            DialogGraph->OnSave();
        }
    }

    FAssetEditorToolkit::SaveAsset_Execute();
}

void FDialogAssetEditor::CreateInternalWidgets()
{
    FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

    FDetailsViewArgs DetailsViewArgs;
    {
        DetailsViewArgs.bLockable = false;
        DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
        DetailsViewArgs.NotifyHook = this;
        DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
    }

    DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
    DetailsView->SetObject(NULL);
    DetailsView->SetIsPropertyEditingEnabledDelegate(FIsPropertyEditingEnabled::CreateSP(this, &FDialogAssetEditor::IsPropertyEditable));
    DetailsView->OnFinishedChangingProperties().AddSP(this, &FDialogAssetEditor::OnFinishedChangingProperties);
}


bool FDialogAssetEditor::IsPropertyEditable() const
{
    TSharedPtr<SGraphEditor> GraphEditor = GraphEditorPtr.Pin();
    return GraphEditor.IsValid() && GraphEditor->GetCurrentGraph() && GraphEditor->GetCurrentGraph()->bEditable;
}

void FDialogAssetEditor::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
}

TSharedRef<SWidget> FDialogAssetEditor::SpawnProperties()
{
    return SNew(SVerticalBox)
            +SVerticalBox::Slot()
            .FillHeight(1.0f)
            .HAlign(HAlign_Fill)
            [
                DetailsView.ToSharedRef()
            ];
}

void FDialogAssetEditor::OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection)
{
    if(!DetailsView.IsValid()) return;

    UE_LOG(LogTemp, Log, TEXT("NewSelection.Num() = %i"), NewSelection.Num());

    if(NewSelection.Num() > 0) DetailsView->SetObjects(NewSelection.Array());
    else DetailsView->SetObject(DialogAsset);
}

void FDialogAssetEditor::NotifyPostChange(const FPropertyChangedEvent& PropertychangedEvent, FProperty* PropertyThatChanged)
{
}

void FDialogAssetEditor::PostUndo(bool bSuccess)
{
    if(!bSuccess) return;

    if(TSharedPtr<SGraphEditor> CurrentGraphEditor = GraphEditorPtr.Pin())
    {
        CurrentGraphEditor->ClearSelectionSet();
        CurrentGraphEditor->NotifyGraphChanged();
    }
    FSlateApplication::Get().DismissAllMenus();
}

void FDialogAssetEditor::PostRedo(bool bSuccess)
{
    if(!bSuccess) return;

    if(TSharedPtr<SGraphEditor> CurrentGraphEditor = GraphEditorPtr.Pin())
    {
        CurrentGraphEditor->ClearSelectionSet();
        CurrentGraphEditor->NotifyGraphChanged();
    }
    FSlateApplication::Get().DismissAllMenus();
}

UDialogAsset* FDialogAssetEditor::GetDialogAsset() const
{
    return DialogAsset;
}

void FDialogAssetEditor::SetGraphEditor(TSharedPtr<class SGraphEditor> GraphEditor)
{
    GraphEditorPtr = GraphEditor;
}
