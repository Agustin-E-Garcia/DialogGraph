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

FDialogNode* UDialogAsset::GetNode(int ID)
{
    if(ID < 0  || ID >= DialogNodes.Num()) return nullptr;

    return &DialogNodes[ID];
}

const FDialogNode* UDialogAsset::GetNode(int ID) const
{
    if(ID < 0  || ID >= DialogNodes.Num()) return nullptr;

    return &DialogNodes[ID];
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
