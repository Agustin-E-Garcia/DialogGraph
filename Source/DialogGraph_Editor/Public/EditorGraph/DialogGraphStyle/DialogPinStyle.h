#pragma once

#include <EdGraphUtilities.h>
#include <KismetPins/SGraphPinColor.h>
#include <EdGraph/EdGraphPin.h>

class SDialogGraphPin : public SGraphPin 
{
public:
	SLATE_BEGIN_ARGS(SDialogGraphPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& inArgs, UEdGraphPin* inGraphPinObj) {
		SGraphPin::Construct(SGraphPin::FArguments(), inGraphPinObj);
	}
	
protected:
	virtual FSlateColor GetPinColor() const override {
		return FSlateColor(FLinearColor(0.2f, 1.0f, 0.2f));
	}
};

struct FDialogPinFactory : public FGraphPanelPinFactory {
public:
	virtual ~FDialogPinFactory() {}
	virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* pin) const override {
		if (FName(TEXT("DialogPin")) == pin->PinType.PinSubCategory) {
			return SNew(SDialogGraphPin, pin);
		}
		return nullptr;
	}
};
