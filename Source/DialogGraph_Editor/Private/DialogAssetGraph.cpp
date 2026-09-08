#include "DialogAssetGraph.h"
#include "DialogAssetGraphNode_Line.h"
#include "DialogNode.h"
#include "EdGraphSchema_DialogAsset.h"
#include "DialogAssetGraphNode_Root.h"

#include "DialogAsset.h"

UDialogAssetGraph::UDialogAssetGraph(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    Schema = UEdGraphSchema_DialogAsset::StaticClass();
    RootNodeClass = UDialogAssetGraphNode_Root::StaticClass();
}

void UDialogAssetGraph::OnCreated()
{
    UDialogAsset* DialogAsset = Cast<UDialogAsset>(GetOuter());
    if(DialogAsset)
    {
    }
}

void UDialogAssetGraph::OnLoaded()
{
    UDialogAsset* DialogAsset = Cast<UDialogAsset>(GetOuter());
    if(DialogAsset)
    {
    }
}

void UDialogAssetGraph::OnSave()
{
    UpdateAsset();
}

void UDialogAssetGraph::UpdateAsset()
{
    TArray<UDialogAssetGraphNode*> nodes;
    GetNodesOfClass(nodes);

    if(nodes.IsEmpty()) return;

    UDialogAsset* DialogAsset = Cast<UDialogAsset>(GetOuter());
    if(!DialogAsset) return;

    DialogAsset->Clear();

    TMap<FGuid, int> GuidToIndex;
    for(int i = 0; i < nodes.Num(); i++)
    {
        if(nodes[i]->GetNodeType() == EDialogNodeType::DEFAULT) continue;
        if(nodes[i]->GetNodeType() == EDialogNodeType::Start) continue;

        FDialogNode* runtimeNode = DialogAsset->CreateNewNode();
        GuidToIndex.Emplace(nodes[i]->NodeGuid, runtimeNode->ID);

        runtimeNode->NodeType = nodes[i]->GetNodeType();
    }

    for(int t = 0; t < nodes.Num(); t++)
    {
        UDialogAssetGraphNode* graphNode = nodes[t];

        if(graphNode->GetNodeType() == EDialogNodeType::DEFAULT) continue;
        if(graphNode->GetNodeType() == EDialogNodeType::Start)
        {
            if(graphNode->GetPinAt(0)->HasAnyConnections())
            {
                DialogAsset->SetStartNodeID(*GuidToIndex.Find(graphNode->GetPinAt(0)->LinkedTo[0]->GetOwningNode()->NodeGuid));
            }

            continue;
        }

        FDialogNode* runtimeNode = DialogAsset->GetNode(GuidToIndex[graphNode->NodeGuid]);
        runtimeNode->NextIDs.Reserve(graphNode->GetAllPins().Num() - 1);

        int outputPinIndex = -1; //since we iterate over ALL the pins and we have no guarantees that the input pin will be the first one, we keep track of which output pin we're looking at, not incrementing when it's an input pin
        for(int q = 0; q < graphNode->GetAllPins().Num(); q++)
        {
            const UEdGraphPin* pin = graphNode->GetAllPins()[q];

            if(pin->Direction == EEdGraphPinDirection::EGPD_Input) continue;
            outputPinIndex++;

            FPinInfo& info = runtimeNode->NextIDs.AddDefaulted_GetRef();

            if(UDialogAssetGraphNode_Line* GraphNode_Line = Cast<UDialogAssetGraphNode_Line>(graphNode)) info.Title = GraphNode_Line->GetDialogLine().ToString();
            else info.Title = pin->GetDefaultAsString();

            if(!pin->HasAnyConnections()) info.NextID = -1;
            else info.NextID = *GuidToIndex.Find(pin->LinkedTo[0]->GetOwningNode()->NodeGuid);
        }
    }
}
