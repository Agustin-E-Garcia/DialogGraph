#include "DialogAssetGraphNode_Root.h"
#include "DialogAssetEditorTypes.h"
#include "Textures/SlateIcon.h"
#include "Styling/AppStyle.h"

UDialogAssetGraphNode_Root::UDialogAssetGraphNode_Root(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDialogAssetGraphNode_Root::AllocateDefaultPins()
{
    CreatePin(EGPD_Output, UDialogAssetEditorTypes::PinCategory_SingleNode, TEXT("Out"));
}

FLinearColor UDialogAssetGraphNode_Root::GetNodeTitleColor() const
{
    return FLinearColor(FColor::Red);
}

FSlateIcon UDialogAssetGraphNode_Root::GetIconAndTint(FLinearColor& OutColor) const
{
    static FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "GraphEditor.Event_16x");
    return Icon;
}

bool UDialogAssetGraphNode_Root::CanUserDeleteNode() const { return false; }
