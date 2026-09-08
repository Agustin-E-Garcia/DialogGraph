#include <DialogAsset.h>

UDialogAsset::UDialogAsset(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) 
{
}

FDialogNode* UDialogAsset::CreateNewNode()
{
    int id = DialogNodes.AddDefaulted();
    FDialogNode* node = &DialogNodes[id];
    node->ID = id;

    return node;
}

FDialogNode* UDialogAsset::GetNode(int Index)
{
    if(Index < 0  || Index >= DialogNodes.Num()) return nullptr;

    return &DialogNodes[Index];
}

FDialogNode* UDialogAsset::GetOrAddNode(int ID)
{
    FDialogNode* node = GetNode(ID);

    if(node != nullptr) return node;
    else return CreateNewNode();

}

void UDialogAsset::Clear()
{
    DialogNodes.Empty();
    StartNodeID = -1;
}
