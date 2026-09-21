#include "DialogAssetGraphNode_Choice.h"
#include "DialogAssetEditorTypes.h"
#include "DialogNode.h"
#include "EdGraph/EdGraphPin.h"
#include "Textures/SlateIcon.h"
#include "Styling/AppStyle.h"
#include "ScopedTransaction.h"
#include "EdGraph/EdGraph.h"

UDialogAssetGraphNode_Choice::UDialogAssetGraphNode_Choice(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDialogAssetGraphNode_Choice::AllocateDefaultPins()
{
    CreatePin(EGPD_Input, UDialogAssetEditorTypes::PinCategory_MultipleNodes, TEXT("In"));
    AddPin();
}

FLinearColor UDialogAssetGraphNode_Choice::GetNodeTitleColor() const
{
    return FLinearColor(FColor::Yellow);
}


FSlateIcon UDialogAssetGraphNode_Choice::GetIconAndTint(FLinearColor& OutColor) const
{
    static FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "GraphEditor.Switch_16x");
    return Icon;
}

void UDialogAssetGraphNode_Choice::ParseToRuntime(FDialogNode* RuntimeNode, const TMap<FGuid, int>& GuidToIndex) const
{
    int outputPinIndex = -1;
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
     }
}

const FChoicePinData* UDialogAssetGraphNode_Choice::GetPinData(const FGuid PinID) const
{
    if(!GuidToPinData.Contains(PinID)) return nullptr;
    return GuidToPinData.Find(PinID);
}

void UDialogAssetGraphNode_Choice::SetPinData(const FGuid PinID, FString InDefaultValue)
{
    if(!GuidToPinData.Contains(PinID)) return;
    GuidToPinData.Find(PinID)->DefaultValue = InDefaultValue;
}

void UDialogAssetGraphNode_Choice::AddPinCondition(const FGuid PinID, TObjectPtr<UClass> InClass, FName InName)
{
    if(!GuidToPinData.Contains(PinID)) return;

    FBindedFunctionData& Data = GuidToPinData.Find(PinID)->Conditions.AddDefaulted_GetRef();
    Data.Class = InClass;
    Data.Name = InName;

    UFunction* Function = InClass->FindFunctionByName(InName);
    if(!Function) return;

    TArray<FPropertyBagPropertyDesc> PropertyBagDescriptions;
    for(TFieldIterator<FProperty> ParamIt(Function); ParamIt; ++ParamIt)
    {
        FProperty* Param = *ParamIt;
        if (!Param->HasAnyPropertyFlags(CPF_Parm)) continue;
        if (Param->HasAnyPropertyFlags(CPF_ReturnParm | CPF_OutParm)) continue;

        PropertyBagDescriptions.Add(FPropertyBagPropertyDesc(Param->GetFName(), Param));
    }

    const UPropertyBag* BagStruct = UPropertyBag::GetOrCreateFromDescs(PropertyBagDescriptions);
    Data.Parameters.InitializeFromBagStruct(BagStruct);
}

void UDialogAssetGraphNode_Choice::AddPin()
{
    UEdGraphPin* pin = CreatePin(EGPD_Output, UDialogAssetEditorTypes::PinCategory_SingleNode, TEXT(""));
    pin->PinType.PinSubCategory = UDialogAssetEditorTypes::PinSubCategory_ChoiceNode;

    if(!GuidToPinData.Contains(pin->PinId)) GuidToPinData.Add(pin->PinId, FChoicePinData());
}

void UDialogAssetGraphNode_Choice::RemovePin(UEdGraphPin* PinToRemove)
{
    if(!PinToRemove) return;

    const FScopedTransaction Transaction(FText::FromString("Remove Pin"));
    Modify();

    UEdGraphNode::RemovePin(PinToRemove);
    if(GuidToPinData.Contains(PinToRemove->PinId)) GuidToPinData.Remove(PinToRemove->PinId);

    GetGraph()->NotifyNodeChanged(this);
}
