#include "SGraphPin_DialogGraph.h"
#include "DialogAssetEditorTypes.h"
#include "DialogAssetGraphNode_Choice.h"
#include "EdGraph/EdGraphSchema.h"
#include "GenericPlatform/GenericApplication.h"
#include "GenericPlatform/ICursor.h"
#include "Math/Color.h"
#include "SGraphPin.h"
#include "ScopedTransaction.h"
#include "Styling/AppStyle.h"
#include "UObject/Linker.h"
#include "Types/SlateEnums.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/SNullWidget.h"

void SGraphPin_DialogGraph::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
{
    //SGraphPin::Construct(SGraphPin::FArguments(), InGraphPinObj);

    this->SetCursor(EMouseCursor::CardinalCross);

    GraphPinObj = InGraphPinObj;
    check(GraphPinObj != NULL);

    const UEdGraphSchema* Schema = GraphPinObj->GetSchema();
    check(Schema);

    SBorder::Construct(SBorder::FArguments()
            .BorderImage(this, &SGraphPin_DialogGraph::GetPinBorder)
            .BorderBackgroundColor(this, &SGraphPin_DialogGraph::GetPinBorderColor)
            .OnMouseButtonDown(this, &SGraphPin_DialogGraph::OnPinMouseDown)
            .Cursor(this, &SGraphPin_DialogGraph::GetPinCursor)
            [
                SAssignNew(PinImage, SImage)
                .Image(GetPinIcon())
                .ColorAndOpacity(GetPinColor())
            ]);
}

const FSlateBrush* SGraphPin_DialogGraph::GetPinIcon() const
{
    return FAppStyle::GetBrush(FName("LandscapeEditor.CircleBrush"));
}

FSlateColor SGraphPin_DialogGraph::GetPinColor() const
{
    return FSlateColor(FLinearColor::White);
}

const FSlateBrush* SGraphPin_DialogGraph::GetPinBorder() const
{
    return FAppStyle::GetBrush(TEXT("Graph.StateNode.Body"));
}

FSlateColor SGraphPin_DialogGraph::GetPinBorderColor() const
{
    return FSlateColor(IsHovered() ? FLinearColor::Yellow : FLinearColor::Transparent);
}

/*
FText SGraphPin_DialogGraph::GetTypeInValue()
{
    UDialogAssetGraphNode_Choice* Node = Cast<UDialogAssetGraphNode_Choice>(GraphPinObj->GetOwningNode());
    if(!Node) return FText();

    return FText::FromString(Node->GetPinData(GraphPinObj->PinId)->DefaultValue);
}

void SGraphPin_DialogGraph::SetTypeInValue(const FText& NewTypeInValue, ETextCommit::Type Type)
{
    if(GraphPinObj->IsPendingKill()) return;

    UDialogAssetGraphNode_Choice* Node = Cast<UDialogAssetGraphNode_Choice>(GraphPinObj->GetOwningNode());
    if(!Node) return;

    if(!Node->GetPinData(GraphPinObj->PinId)->GetDefaultValueAsText().EqualTo(NewTypeInValue))
    {
        const FScopedTransaction Transaction(FText::FromString("Change Choice Pin Value"));
        GraphPinObj->Modify();

        Node->SetPinData(GraphPinObj->PinId, NewTypeInValue.ToString());
    }
}

const FSlateBrush* SGraphPin_DialogGraph::GetConditionLockImage()
{
    UDialogAssetGraphNode_Choice* Node = Cast<UDialogAssetGraphNode_Choice>(GraphPinObj->GetOwningNode());
    const FChoicePinData* PinData = Node ? Node->GetPinData(GraphPinObj->PinId) : nullptr;

    const bool bHasConditions = PinData && PinData->Conditions.Num() > 0;
    return FAppStyle::GetBrush(bHasConditions ? "Sequencer.LockSequence" : "Sequencer.UnlockSequence");
}*/
