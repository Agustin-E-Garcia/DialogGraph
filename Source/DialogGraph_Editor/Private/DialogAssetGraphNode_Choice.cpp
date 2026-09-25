#include "DialogAssetGraphNode_Choice.h"
#include "Math/Color.h"
#include "Textures/SlateIcon.h"
#include "Styling/AppStyle.h"

UDialogAssetGraphNode_Choice::UDialogAssetGraphNode_Choice(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

FSlateIcon UDialogAssetGraphNode_Choice::GetIconAndTint(FLinearColor& OutColor) const 
{
    static FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "GraphEditor.Switch_16x");
    return Icon;
}

FLinearColor UDialogAssetGraphNode_Choice::GetNodeTitleColor() const
{
    return FLinearColor::Yellow;
}
