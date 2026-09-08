#include "DialogAssetGraphNode_Choice.h"
#include "DialogAssetEditorTypes.h"
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
    UEdGraphPin* pin = CreatePin(EGPD_Output, UDialogAssetEditorTypes::PinCategory_SingleNode, TEXT(""));
    pin->PinType.PinSubCategory = UDialogAssetEditorTypes::PinSubCategory_ChoiceNode;
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

void UDialogAssetGraphNode_Choice::AddPin()
{
    UEdGraphPin* pin = CreatePin(EGPD_Output, UDialogAssetEditorTypes::PinCategory_SingleNode, TEXT(""));
    pin->PinType.PinSubCategory = UDialogAssetEditorTypes::PinSubCategory_ChoiceNode;
}

void UDialogAssetGraphNode_Choice::RemovePin(UEdGraphPin* PinToRemove)
{
    if(!PinToRemove) return;

    const FScopedTransaction Transaction(FText::FromString("Remove Pin"));
    Modify();

    UEdGraphNode::RemovePin(PinToRemove);

    GetGraph()->NotifyNodeChanged(this);
}
