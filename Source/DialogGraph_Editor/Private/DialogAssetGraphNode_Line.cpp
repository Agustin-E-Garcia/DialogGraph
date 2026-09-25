#include "DialogAssetGraphNode_Line.h"
#include "EdGraph/EdGraphNode.h"
#include "Textures/SlateIcon.h"
#include "Styling/AppStyle.h"
#include "EdGraph/EdGraphPin.h"

UDialogAssetGraphNode_Line::UDialogAssetGraphNode_Line(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

FSlateIcon UDialogAssetGraphNode_Line::GetIconAndTint(FLinearColor& OutColor) const
{
    static FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "Icons.Comment");
    return Icon;
}

void UDialogAssetGraphNode_Line::ParseToRuntime(FDialogNode* RuntimeNode, const TMap<FGuid, int>& GuidToIndex) const
{
    /*for(const UEdGraphPin* Pin : GetAllPins())
    {
        if(Pin->Direction != EEdGraphPinDirection::EGPD_Output) continue;

        FPinInfo& info = RuntimeNode->NextIDs.AddDefaulted_GetRef();
        info.Title = GetDialogLine().ToString();
        info.ConditionsData = Conditions;

        if(!Pin->HasAnyConnections()) info.NextID = -1;
        else info.NextID = *GuidToIndex.Find(Pin->LinkedTo[0]->GetOwningNode()->NodeGuid);
    }*/
}


