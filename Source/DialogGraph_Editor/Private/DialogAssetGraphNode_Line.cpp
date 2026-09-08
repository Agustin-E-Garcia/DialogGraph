#include "DialogAssetGraphNode_Line.h"
#include "Textures/SlateIcon.h"
#include "Styling/AppStyle.h"

UDialogAssetGraphNode_Line::UDialogAssetGraphNode_Line(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

FSlateIcon UDialogAssetGraphNode_Line::GetIconAndTint(FLinearColor& OutColor) const
{
    static FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "Icons.Comment");
    return Icon;
}
