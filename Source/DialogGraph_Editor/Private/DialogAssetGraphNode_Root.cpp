#include "DialogAssetGraphNode_Root.h"
#include "DialogAssetEditorTypes.h"
#include "Textures/SlateIcon.h"
#include "Styling/AppStyle.h"

UDialogAssetGraphNode_Root::UDialogAssetGraphNode_Root(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

EDialogNodeType UDialogAssetGraphNode_Root::GetNodeType() const
{
    return EDialogNodeType::Start;
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

FText UDialogAssetGraphNode_Root::GetNodeTitle(ENodeTitleType::Type titleType) const
{
    return FText::FromString(TEXT("On Dialog Start"));
}

bool UDialogAssetGraphNode_Root::CanUserDeleteNode() const { return false; }
bool UDialogAssetGraphNode_Root::CanDuplicateNode() const { return false; }

