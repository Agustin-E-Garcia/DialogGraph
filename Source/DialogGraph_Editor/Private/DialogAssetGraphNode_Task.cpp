#include "DialogAssetGraphNode_Task.h"
#include "CoreGlobals.h"
#include "EdGraph/EdGraphPin.h"
#include "StructUtils/PropertyBag.h"
#include "Textures/SlateIcon.h"
#include "Styling/AppStyle.h"
#include "UObject/ObjectMacros.h"
#include "UObject/UnrealType.h"

UDialogAssetGraphNode_Task::UDialogAssetGraphNode_Task(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

FLinearColor UDialogAssetGraphNode_Task::GetNodeTitleColor() const
{
    return FLinearColor(FColor::Green);
}

FSlateIcon UDialogAssetGraphNode_Task::GetIconAndTint(FLinearColor& OutColor) const
{
    static FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "ClassIcon.BlueprintFunctionLibrary");
    return Icon;
}

void UDialogAssetGraphNode_Task::ParseToRuntime(FDialogNode* RuntimeNode, const TMap<FGuid, int>& GuidToIndex) const
{
    RuntimeNode->BindedTaskFunction = TaskFunctionData;

    for(const UEdGraphPin* Pin : GetAllPins())
    {
        if(Pin->Direction != EEdGraphPinDirection::EGPD_Output) continue;

        FPinInfo& Info = RuntimeNode->NextIDs.AddDefaulted_GetRef();
        if(!Pin->HasAnyConnections()) Info.NextID = -1;
        else Info.NextID = *GuidToIndex.Find(Pin->LinkedTo[0]->GetOwningNode()->NodeGuid);
    }
}

void UDialogAssetGraphNode_Task::SetFunctionData(TObjectPtr<UClass> FunctionClass, FName FunctionName)
{
    TaskFunctionData.Class = FunctionClass;
    TaskFunctionData.Name = FunctionName;

    InitializeBindedFunction(TaskFunctionData);
}
