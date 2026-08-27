#pragma once

#include <EdGraphUtilities.h>
#include <KismetPins/SGraphPinColor.h>
#include <EdGraph/EdGraphPin.h>

#include <Widgets/Input/SMultiLineEditableTextBox.h>
#include <EditorGraph/GraphNodes/DialogGraphNode_Choice.h>

class SChoiceGraphPin : public SGraphPin 
{
public:
	SLATE_BEGIN_ARGS(SChoiceGraphPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& inArgs, UEdGraphPin* inGraphPinObj)
    {
		SGraphPin::Construct(SGraphPin::FArguments(), inGraphPinObj);
	}
	
protected:
	virtual FSlateColor GetPinColor() const override 
    {
		return FSlateColor(FLinearColor(0.2f, 1.0f, 0.2f));
	}

    virtual TSharedRef<SWidget> GetLabelWidget(const FName& InPinLableStyle) override
    {
        return SNew(SBox)
                .MinDesiredWidth(18.0f)
                .MaxDesiredHeight(200.0f)
                [
                    SNew(SMultiLineEditableTextBox)
                        .Style(FAppStyle::Get(), "Graph.EditableTextBox")
                        .Text(FText::FromName(GetPinObj()->PinName))
                        .OnTextCommitted(this, &SChoiceGraphPin::OnOptionTextCommited)
                        .SelectAllTextWhenFocused(true)
                        .ForegroundColor(FSlateColor::UseForeground())
                ];
    }

private:
    void OnOptionTextCommited(const FText& NewText, ETextCommit::Type CommitType)
    {
        UDialogGraphNode_Choice* node = Cast<UDialogGraphNode_Choice>(GetPinObj()->GetOwningNode());
        if(!node) return;

        node->SetPinOption(GetPinObj()->PinId, NewText.ToString());
        GetPinObj()->PinName = *NewText.ToString();
    }

    FText _pinTitle = FText::FromString("");
};

struct FChoicePinFactory : public FGraphPanelPinFactory {
public:
	virtual ~FChoicePinFactory() {}
	virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* pin) const override 
    {
		if (FName(TEXT("ChoicePin")) == pin->PinType.PinSubCategory)
        {
			return SNew(SChoiceGraphPin, pin);
		}
		return nullptr;
	}
};
