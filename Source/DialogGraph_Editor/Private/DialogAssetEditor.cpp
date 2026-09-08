#include "DialogAssetEditor.h"
#include "DialogAssetGraph.h"
#include "DialogGraph_Editor.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphSchema.h"
#include "Framework/Commands/UIAction.h"
#include "Framework/Commands/UICommandList.h"
#include "Framework/Docking/TabManager.h"
#include "GraphEditor.h"
#include "Logging/LogCategory.h"
#include "Logging/LogMacros.h"
#include "Modules/ModuleManager.h"
#include "ScopedTransaction.h"
#include "Templates/SharedPointer.h"
#include "Templates/SubclassOf.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "UObject/UObjectGlobals.h"
#include "DialogAssetEditorApplicationMode.h"
#include "DialogAsset.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Framework/Commands/GenericCommands.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

const FName FDialogAssetEditor::DialogGraphMode(TEXT("DialogGraph"));

FDialogAssetEditor::FDialogAssetEditor()
{
    DialogAsset = nullptr;

    GraphClass = UDialogAssetGraph::StaticClass();
    GraphName = "Dialog Asset";
}

FDialogAssetEditor::~FDialogAssetEditor()
{
}

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

    const FGraphPanelSelectionSet SelectedNodes = CurrentGraphEditor->GetSelectedNodes();
}
bool FDialogAssetEditor::CanCopySelectedNodes() { return true; }

void FDialogAssetEditor::CutSelectedNodes() {}
bool FDialogAssetEditor::CanCutSelectedNodes() { return true; }

void FDialogAssetEditor::PasteSelectedNodes() {}
bool FDialogAssetEditor::CanPasteSelectedNodes() { return true; }

void FDialogAssetEditor::DuplicateSelectedNodes() {}
bool FDialogAssetEditor::CanDuplicateSelectedNodes() { return true; }

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
