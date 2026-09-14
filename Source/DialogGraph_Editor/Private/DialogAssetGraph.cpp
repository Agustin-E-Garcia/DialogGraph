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

        graphNode->ParseToRuntime(runtimeNode, GuidToIndex);
    }
}
