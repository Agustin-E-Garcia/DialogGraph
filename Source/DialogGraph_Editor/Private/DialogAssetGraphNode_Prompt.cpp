#include "DialogAssetGraphNode_Prompt.h"
#include "DialogAssetEditorTypes.h"
#include "DialogNode.h"
#include "EdGraph/EdGraphNode.h"
#include "Textures/SlateIcon.h"
#include "Styling/AppStyle.h"

UDialogAssetGraphNode_Prompt::UDialogAssetGraphNode_Prompt(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDialogAssetGraphNode_Prompt::AllocateDefaultPins()
{
    CreatePin(EGPD_Input, UDialogAssetEditorTypes::PinCategory_MultipleNodes, TEXT("In"));
    CreatePin(EGPD_Output, UDialogAssetEditorTypes::PinCategory_MultipleNodes, TEXT("Out"));
}

FLinearColor UDialogAssetGraphNode_Prompt::GetNodeTitleColor() const
{
    return FLinearColor(FColor::Yellow);
}


FSlateIcon UDialogAssetGraphNode_Prompt::GetIconAndTint(FLinearColor& OutColor) const
{
    static FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "GraphEditor.Switch_16x");
    return Icon;
}

void UDialogAssetGraphNode_Prompt::ParseToRuntime(FDialogNode* RuntimeNode, const TMap<FGuid, int>& GuidToIndex) const
{
    /*int outputPinIndex = -1;
    for(int q = 0; q < GetAllPins().Num(); q++)
    {
        const UEdGraphPin* pin = GetAllPins()[q];

        if(pin->Direction == EEdGraphPinDirection::EGPD_Input) continue;
        outputPinIndex++;

        FPinInfo& info = RuntimeNode->NextIDs.AddDefaulted_GetRef();
        info.Title = GuidToPinData.Find(pin->PinId)->DefaultValue;
        info.ConditionsData = GuidToPinData.Find(pin->PinId)->Conditions;

        if(!pin->HasAnyConnections()) info.NextID = -1;
        else info.NextID = *GuidToIndex.Find(pin->LinkedTo[0]->GetOwningNode()->NodeGuid);
     }*/
}
