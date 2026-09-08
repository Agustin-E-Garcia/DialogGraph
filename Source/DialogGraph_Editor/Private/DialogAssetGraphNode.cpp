#include "DialogAssetGraphNode.h"
#include "DialogAssetEditorTypes.h"

UDialogAssetGraphNode::UDialogAssetGraphNode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDialogAssetGraphNode::AllocateDefaultPins()
{
    CreatePin(EGPD_Input, UDialogAssetEditorTypes::PinCategory_MultipleNodes, TEXT("In"));
    CreatePin(EGPD_Output, UDialogAssetEditorTypes::PinCategory_SingleNode, TEXT("Out"));
}

FLinearColor UDialogAssetGraphNode::GetNodeTitleColor() const
{
    return FLinearColor(FColor::Blue);
}

bool UDialogAssetGraphNode::ShowPaletteIconOnNode() const
{
    return true;
}

void UDialogAssetGraphNode::GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const 
{
    Super::GetNodeContextMenuActions(Menu, Context);
}

bool UDialogAssetGraphNode::CanUserDeleteNode() const { return true; }
