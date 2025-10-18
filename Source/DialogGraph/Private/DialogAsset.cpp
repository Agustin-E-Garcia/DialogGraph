#include <DialogAsset.h>

bool UDialogAsset::CreateNewNode()
{
    FDialogNode NewNode;
    NewNode.ID = _NextNodeID++;

    DialogNodes.Add(NewNode.ID, NewNode);
    return true;
}

bool UDialogAsset::DeleteNode(int ID)
{
    if(!DialogNodes.Contains(ID)) return false;

    DialogNodes.Remove(ID);
    return true;
}
