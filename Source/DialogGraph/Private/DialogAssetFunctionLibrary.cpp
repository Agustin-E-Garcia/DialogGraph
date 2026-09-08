#include "DialogAssetFunctionLibrary.h"
#include "DialogAsset.h"

void UDialogAssetFunctionLibrary::GetNodeType(const FDialogNode& Node, EDialogNodeType& OutType)
{
    OutType = Node.NodeType;
}

void UDialogAssetFunctionLibrary::GetLineText(const FDialogNode& Node, FText& OutText)
{
    OutText = Node.NextIDs.Num() > 0 ? FText::FromString(Node.NextIDs[0].Title) : FText::GetEmpty();
}

void UDialogAssetFunctionLibrary::GetChoiceOptions(const FDialogNode& Node, TArray<FPinInfo>& OutOptions) { OutOptions = Node.NextIDs; }
void UDialogAssetFunctionLibrary::GetOptionText(const FPinInfo& Option, FText& OutText) { OutText = FText::FromString(Option.Title); }

void UDialogAssetFunctionLibrary::GetStartingNode(UDialogAsset* Asset, FDialogNode& OutStartNode, int32& OutStartIndex, EDialogFlowResult& Branches)
{
    const FDialogNode* node = Asset->GetNode(Asset->GetStartNodeID());

    if(!node)
    {
        Branches = EDialogFlowResult::End;
        OutStartIndex = -1;
        return;
    }

    OutStartIndex = Asset->GetStartNodeID();
    OutStartNode = *node;
    Branches = EDialogFlowResult::HasNext;
}

void UDialogAssetFunctionLibrary::AdvanceDialog(UDialogAsset* Asset, int32 CurrentIndex, int32 ChoiceIndex, FDialogNode& OutNextNode, int32& OutNextIndex, EDialogFlowResult& Branches)
{
    const FDialogNode& current = *Asset->GetNode(CurrentIndex);

    const int32 NextID = current.NextIDs[ChoiceIndex].NextID;

    if (NextID == -1)
    {
        Branches = EDialogFlowResult::End;
        OutNextIndex = -1;
        return;
    }

    OutNextIndex = NextID;
    OutNextNode = *Asset->GetNode(NextID);
    Branches = EDialogFlowResult::HasNext;
}
