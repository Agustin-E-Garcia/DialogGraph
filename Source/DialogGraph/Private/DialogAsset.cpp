#include "DialogNode.h"
#include <DialogAsset.h>

FDialogNode* UDialogAsset::CreateNewNode(NodeType nodeType)
{
    FDialogNode* newNode = &DialogNodes.Emplace(_NextNodeID, FDialogNode(nodeType, _NextNodeID));

    if(nodeType == NodeType::Start && StartNode == -1) StartNode = _NextNodeID;

    _NextNodeID++;
    return newNode;
}

FDialogNode* UDialogAsset::GetNode(int ID)
{
    return DialogNodes.Find(ID);
}

bool UDialogAsset::DeleteNode(int ID)
{
    if(!DialogNodes.Contains(ID)) return false;
    if(StartNode == ID) StartNode = -1;
    DialogNodes.Remove(ID);
    return true;
}

FDialogNode* UDialogAsset::GetOrAddNode(int ID, NodeType nodeType)
{
    FDialogNode* node = GetNode(ID);

    if(node != nullptr) return node;
    else return CreateNewNode(nodeType);

}
