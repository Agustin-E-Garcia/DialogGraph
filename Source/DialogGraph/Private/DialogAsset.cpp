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

void UDialogAsset::SetStartNodeID(int index)
{
    StartNodeID = index;
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

int UDialogAsset::GetStartNodeID() const 
{ 
    return StartNodeID; 
}

int UDialogAsset::GetNodeCount() const 
{
    return DialogNodes.Num(); 
}

FDialogNode* UDialogAsset::GetOrAddNode(int ID)
{
    FDialogNode* node = GetNode(ID);

    if(node != nullptr) return node;
    else return CreateNewNode();
}

bool UDialogAsset::IsEmpty() const
{
    return DialogNodes.IsEmpty();
}

void UDialogAsset::Clear()
{
    DialogNodes.Empty();
    StartNodeID = -1;
}
