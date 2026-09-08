#include "SGraphPin_DialogGraph.h"
#include "DialogAssetEditorTypes.h"
#include "GenericPlatform/GenericApplication.h"
#include "Math/Color.h"
#include "SGraphPin.h"
#include "ScopedTransaction.h"
#include "Styling/AppStyle.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Images/SImage.h"

void SGraphPin_DialogGraph::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
{
    SGraphPin::Construct(SGraphPin::FArguments(), InGraphPinObj);
}

TSharedRef<SWidget> SGraphPin_DialogGraph::GetLabelWidget(const FName& InPinLabelStyle)
{
    if(GraphPinObj->PinType.PinSubCategory != UDialogAssetEditorTypes::PinSubCategory_ChoiceNode)
        return SGraphPin::GetLabelWidget(InPinLabelStyle);

    return SNew(SHorizontalBox)
            +SHorizontalBox::Slot()
            .AutoWidth()
            .VAlign(VAlign_Center)
            .Padding(5.0f)
            [
                SNew(SImage)
            ]
            +SHorizontalBox::Slot()
            .AutoWidth()
            .VAlign(VAlign_Center)
            .Padding(5.0f)
            .MinWidth(50.0f)
            [
                SNew(SMultiLineEditableTextBox)
                .Style(FAppStyle::Get(), "Graph.EditableTextBox")
                .Text(GetTypeInValue())
                .SelectAllTextWhenFocused(true)
                .OnTextCommitted(this, &SGraphPin_DialogGraph::SetTypeInValue)
                .ForegroundColor(FSlateColor::UseForeground())
                .WrapTextAt(400.0f)
                .ModiferKeyForNewLine(EModifierKey::Shift)
            ];
}

FSlateColor SGraphPin_DialogGraph::GetPinColor() const
{
    return FSlateColor(FLinearColor(0.2f, 1.0f, 0.2f));
}

FText SGraphPin_DialogGraph::GetTypeInValue()
{
    return GraphPinObj->GetDefaultAsText();
}

void SGraphPin_DialogGraph::SetTypeInValue(const FText& NewTypeInValue, ETextCommit::Type Type)
{
    if(GraphPinObj->IsPendingKill()) return;

    if(!GraphPinObj->GetDefaultAsText().EqualTo(NewTypeInValue))
    {
        const FScopedTransaction Transaction(FText::FromString("Change Choice Pin Value"));
        GraphPinObj->Modify();

        GraphPinObj->GetSchema()->TrySetDefaultText(*GraphPinObj, NewTypeInValue);
    }
}
