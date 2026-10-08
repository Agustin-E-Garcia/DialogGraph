#include "DialogAssetGraphNode_Prompt.h"
#include "DialogAssetEditorTypes.h"
#include "DialogAssetGraphNode_Choice.h"
#include "DialogNode.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphPin.h"
#include "Textures/SlateIcon.h"
#include "Styling/AppStyle.h"

UDialogAssetGraphNode_Prompt::UDialogAssetGraphNode_Prompt(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

EDialogNodeType UDialogAssetGraphNode_Prompt::GetNodeType() const
{
    return EDialogNodeType::Prompt;
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

FText UDialogAssetGraphNode_Prompt::GetNodeTitle(ENodeTitleType::Type titleType) const
{
    return FText::FromString(TEXT("Prompt"));
}

void UDialogAssetGraphNode_Prompt::ParseToRuntime(FDialogNode* RuntimeNode, const TMap<FGuid, int>& GuidToIndex) const
{
    for(UEdGraphPin* Pin : GetAllPins())
    {
        if(Pin->Direction == EEdGraphPinDirection::EGPD_Input) continue;
        if(!Pin->HasAnyConnections()) continue;

        for(int ConnectionID = 0; ConnectionID < Pin->LinkedTo.Num(); ConnectionID++)
        {
            UDialogAssetGraphNode_Choice* ChoiceNode = Cast<UDialogAssetGraphNode_Choice>(Pin->LinkedTo[ConnectionID]->GetOwningNode());
            if(!ChoiceNode) continue;

            FPinInfo& info = RuntimeNode->NextIDs.AddDefaulted_GetRef();
            info.Title = ChoiceNode->GetDialogLine().ToString();
            info.ConditionsData = ChoiceNode->GetConditionsData();

            const UEdGraphPin* ChoicePin = ChoiceNode->GetOutputPin();
            if(!ChoicePin) continue;

            if(!ChoicePin->HasAnyConnections()) info.NextID = -1;
            else info.NextID = *GuidToIndex.Find(ChoicePin->LinkedTo[0]->GetOwningNode()->NodeGuid);
        }
    }
}
