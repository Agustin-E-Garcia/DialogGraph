#include "DialogAssetGraphNode_Choice.h"
#include "Math/Color.h"
#include "Textures/SlateIcon.h"
#include "Styling/AppStyle.h"

UDialogAssetGraphNode_Choice::UDialogAssetGraphNode_Choice(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

EDialogNodeType UDialogAssetGraphNode_Choice::GetNodeType() const 
{
    return EDialogNodeType::Choice;
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

FText UDialogAssetGraphNode_Choice::GetNodeTitle(ENodeTitleType::Type titleType) const
{
    return FText::FromString(TEXT("Choice"));
}
