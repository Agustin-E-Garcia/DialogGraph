#include "DialogAssetGraphNode.h"
#include "DialogAssetEditorTypes.h"
#include "EdGraph/EdGraphSchema.h"
#include "SGraphNode.h"
#include "EdGraphSchema_DialogAsset.h"

UDialogAssetGraphNode::UDialogAssetGraphNode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDialogAssetGraphNode::AllocateDefaultPins()
{
    CreatePin(EGPD_Input, UDialogAssetEditorTypes::PinCategory_MultipleNodes, TEXT("In"));
    CreatePin(EGPD_Output, UDialogAssetEditorTypes::PinCategory_SingleNode, TEXT("Out"));
}

FLinearColor UDialogAssetGraphNode::GetNodeTitleColor() const
{
    return FLinearColor(FColor::Blue);
}

bool UDialogAssetGraphNode::ShowPaletteIconOnNode() const
{
    return true;
}

void UDialogAssetGraphNode::GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const 
{
    Super::GetNodeContextMenuActions(Menu, Context);
}

bool UDialogAssetGraphNode::CanUserDeleteNode() const { return true; }

void UDialogAssetGraphNode::AutowireNewNode(UEdGraphPin* FromPin)
{
    if(!FromPin || Pins.Num() <= 0) return;

    const UEdGraphSchema_DialogAsset* Schema = Cast<UEdGraphSchema_DialogAsset>(GetSchema());
    if(Schema)
    {
        FPinConnectionResponse ConnectionResponse = Schema->CanCreateConnection(FromPin, Pins[0]);
        if(ConnectionResponse.Response == ECanCreateConnectionResponse::CONNECT_RESPONSE_MAKE)
        {
            FromPin->MakeLinkTo(Pins[0]);
        }
    }
}

void UDialogAssetGraphNode::AddCondition(TObjectPtr<UClass> InClass, FName InName)
{
    FBindedFunctionData& Data = Conditions.AddDefaulted_GetRef();
    Data.Class = InClass;
    Data.Name = InName;

    InitializeBindedFunction(Data);

    if(VisualGraphNode) VisualGraphNode->UpdateGraphNode();
}

void UDialogAssetGraphNode::InitializeBindedFunction(FBindedFunctionData& FunctionData)
{
    UFunction* Function = FunctionData.Class->FindFunctionByName(FunctionData.Name);
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
    FunctionData.Parameters.InitializeFromBagStruct(BagStruct);
}
