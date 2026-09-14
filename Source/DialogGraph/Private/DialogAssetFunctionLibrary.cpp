#include "DialogAssetFunctionLibrary.h"
#include "DialogAsset.h"
#include "DialogNode.h"
#include "StructUtils/PropertyBag.h"
#include "UObject/StructOnScope.h"

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

void UDialogAssetFunctionLibrary::GetStartingNode(const UDialogAsset* Asset, FDialogNode& OutStartNode, int32& OutStartIndex, EDialogFlowResult& Branches)
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

/** Returns the next dialog node and that node's index, executing Task nodes in the way */
void UDialogAssetFunctionLibrary::AdvanceDialog(const UDialogAsset* Asset, int32 CurrentIndex, int32 ChoiceIndex, FDialogNode& OutNextNode, int32& OutNextIndex, EDialogFlowResult& Branches)
{
    const FDialogNode& current = *Asset->GetNode(CurrentIndex);

    int32 NextID = current.NextIDs[ChoiceIndex].NextID;

    if (NextID == -1)
    {
        Branches = EDialogFlowResult::End;
        OutNextIndex = -1;
        return;
    }

    const FDialogNode* NextNode = Asset->GetNode(NextID);

    while(NextNode->NodeType == EDialogNodeType::Task)
    {
        ExecuteTask(NextNode);

        NextID = NextNode->NextIDs[0].NextID;
        if(NextID == -1)
        {
            Branches = EDialogFlowResult::End;
            OutNextIndex = -1;
            return;
        }

        NextNode = Asset->GetNode(NextID);
    }

    OutNextIndex = NextID;
    OutNextNode = *NextNode;
    Branches = EDialogFlowResult::HasNext;
}

void UDialogAssetFunctionLibrary::ExecuteTask(const FDialogNode* TaskNode)
{
    UFunction* Function = TaskNode->FunctionClass->FindFunctionByName(TaskNode->FunctionName);
    UObject* Target = Function->GetOuterUClass()->GetDefaultObject();
    if(Function->NumParms == 0)
    {
        Target->ProcessEvent(Function, nullptr);
        return;
    }

    FStructOnScope FuncParams(Function);
    uint8* Params = FuncParams.GetStructMemory();

    const UPropertyBag* BagStruct = TaskNode->FunctionProperties.GetPropertyBagStruct();
    const uint8* BagMemory = TaskNode->FunctionProperties.GetValue().GetMemory();

    for(const FPropertyBagPropertyDesc& Desc : BagStruct->GetPropertyDescs())
    {
        FProperty* DestProp = Function->FindPropertyByName(Desc.Name);
        if(!DestProp) continue;

        const void* SrcAddr = Desc.CachedProperty->ContainerPtrToValuePtr<void>(BagMemory);
        void* DestAddr = DestProp->ContainerPtrToValuePtr<void>(Params);

        DestProp->CopyCompleteValue(DestAddr, SrcAddr);
    }

    Target->ProcessEvent(Function, Params);
}
