#include "EdGraph/EdGraph.h"
#include <DialogAssetEditor.h>
#include <EditorGraph/DialogGraphEditMode.h>
#include <EditorGraph/DialogGraphSchema.h>
#include <DialogAsset.h>
#include <Kismet2/BlueprintEditorUtils.h>
#include <EditorGraph/GraphNodes/DialogGraphNode_Start.h>
#include <EditorGraph/GraphNodes/DialogGraphNode_Line.h>
#include <EditorGraph/GraphNodes/DialogGraphNode_Choice.h>
#include <GraphEditAction.h>
#include <DialogNode.h>

void FDialogAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& tabManager)
{
    FWorkflowCentricApplication::RegisterTabSpawners(tabManager);
}

void FDialogAssetEditor::InitEditor(const EToolkitMode::Type mode, const TSharedPtr<IToolkitHost>& initToolkitHost, UObject* inObject)
{
    _WorkingAsset = Cast<UDialogAsset>(inObject);
    _WorkingGraph = FBlueprintEditorUtils::CreateNewGraph(_WorkingAsset, *_WorkingAsset->GetName(), UEdGraph::StaticClass(), UDialogGraphSchema::StaticClass());

    TSharedPtr<FExtender> compileExtension = MakeShareable(new FExtender);

    compileExtension->AddToolBarExtension
        (
            TEXT("Asset"),
            EExtensionHook::After,
            GetToolkitCommands(),
            FToolBarExtensionDelegate::CreateSP(this, &FDialogAssetEditor::FillToolbar)
        );

    AddToolbarExtender(compileExtension);

    InitAssetEditor(mode, initToolkitHost, TEXT("DialogAssetEditor"), FTabManager::FLayout::NullLayout, true, true, inObject);

    AddApplicationMode(TEXT("DialogGraphEditMode"), MakeShareable(new FDialogGraphEditMode(SharedThis(this))));
    SetCurrentMode(TEXT("DialogGraphEditMode"));

    _OnGraphChangedHandle = _WorkingGraph->AddOnGraphChangedHandler(FOnGraphChanged::FDelegate::CreateSP(this, &FDialogAssetEditor::OnGraphChanged));

    InitializeGraphFromAsset();
}

void FDialogAssetEditor::FillToolbar(FToolBarBuilder& Builder)
{
    Builder.AddToolBarButton
    (
        FUIAction(FExecuteAction::CreateSP(this, &FDialogAssetEditor::UpdateAssetFromGraph)),
        NAME_None,
        FText::FromString("Compile"),
        FText::FromString("Compile the dialog graph into runtime data."),
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "Kismet.Compile")
    );
}

void FDialogAssetEditor::OnClose() 
{
    _WorkingGraph->RemoveOnGraphChangedHandler(_OnGraphChangedHandle);
    UpdateAssetFromGraph();
}

void FDialogAssetEditor::OnGraphChanged(const FEdGraphEditAction& editAction)
{
    if(editAction.Action == EEdGraphActionType::GRAPHACTION_RemoveNode)
    {
        for(const UEdGraphNode* node : editAction.Nodes)
        {
            const UDialogGraphNode_Base* graphNode = Cast<UDialogGraphNode_Base>(node);
            if(graphNode == nullptr) continue;

            _RemovedNodes.Add(graphNode->runtimeNodeID);
        }
    }
}

void FDialogAssetEditor::InitializeGraphFromAsset()
{
    if(_WorkingAsset->IsEmpty())
    {
        UDialogGraphNode_Start* startNode = NewObject<UDialogGraphNode_Start>(_WorkingGraph);
        FEditorData editorData;
        startNode->SetupNode(nullptr, &editorData);

        _WorkingGraph->AddNode(startNode, true, true);
        return;
    }

    CreateGraphNode(_WorkingAsset->GetStartNodeID(), nullptr);
}

void FDialogAssetEditor::UpdateAssetFromGraph()
{
    TArray<UDialogGraphNode_Start*> nodes;
    _WorkingGraph->GetNodesOfClass(nodes);

    if(nodes[0] == nullptr) return;

    if(!_RemovedNodes.IsEmpty())
    {
        for (int id : _RemovedNodes)
        {
            _WorkingAsset->DeleteNode(id);
        }
        _RemovedNodes.Empty();
    }

    CreateRuntimeNode(nodes[0]);
    // TODO: Fix issue where adding a choice node sometimes will empty all the other nodes nextID, running this a second time seems to fix the issue for some reason
    CreateRuntimeNode(nodes[0]);
}

void FDialogAssetEditor::CreateGraphNode(int runtimeNodeID, UEdGraphPin* fromPin)
{
    const FDialogNode* runtimeNode = _WorkingAsset->GetNode(runtimeNodeID);
    if(runtimeNode == nullptr) return;

    UDialogGraphNode_Base* graphNode;
    switch (runtimeNode->NodeType)
    {
        case NodeType::Start:
            graphNode = NewObject<UDialogGraphNode_Start>(_WorkingGraph);
            break;
        case NodeType::Line:
            graphNode = NewObject<UDialogGraphNode_Line>(_WorkingGraph);
            Cast<UDialogGraphNode_Line>(graphNode)->SetDialogLine(FText::FromString(runtimeNode->DialogText));
            break;
        case NodeType::Choice:
            graphNode = NewObject<UDialogGraphNode_Choice>(_WorkingGraph);
            break;
    }

    graphNode->SetupNode(fromPin, &runtimeNode->editorData, true);
    graphNode->runtimeNodeID = runtimeNodeID;

    _WorkingGraph->AddNode(graphNode, true, true);

    for (int i = 0; i < runtimeNode->NextIDs.Num(); i++)
    {
        CreateGraphNode(runtimeNode->NextIDs[i], graphNode->GetPinWithDirectionAt(i, EEdGraphPinDirection::EGPD_Output));
    }
}

int FDialogAssetEditor::CreateRuntimeNode(UDialogGraphNode_Base* graphNode)
{
    if(graphNode == nullptr) return -1;

    NodeType nodeType;

    if (Cast<UDialogGraphNode_Start>(graphNode) != nullptr) nodeType = NodeType::Start;
    if (Cast<UDialogGraphNode_Line>(graphNode) != nullptr) nodeType = NodeType::Line;
    if (Cast<UDialogGraphNode_Choice>(graphNode) != nullptr) nodeType = NodeType::Choice;

    FDialogNode* runtimeNode = _WorkingAsset->GetOrAddNode(graphNode->runtimeNodeID, nodeType);
    graphNode->runtimeNodeID = runtimeNode->ID;

    if(nodeType == NodeType::Line)
    {
        runtimeNode->DialogText = Cast<UDialogGraphNode_Line>(graphNode)->GetDialogLine().ToString();
    }

    runtimeNode->editorData = FEditorData
        (
            graphNode->GetOutputPinCount(),
            graphNode->GetPosition(),
            graphNode->NodeComment
        );

    runtimeNode->NextIDs.Empty();
    for (int i = 0; i < graphNode->GetAllPins().Num(); i++)
    {
        if(graphNode->GetAllPins()[i]->Direction == EEdGraphPinDirection::EGPD_Input) continue;
        if(!graphNode->GetAllPins()[i]->HasAnyConnections()) continue;

        int id = CreateRuntimeNode(Cast<UDialogGraphNode_Base>(graphNode->GetAllPins()[i]->LinkedTo[0]->GetOwningNode()));
        runtimeNode->NextIDs.Add(id);
    }

    return runtimeNode->ID;
}
