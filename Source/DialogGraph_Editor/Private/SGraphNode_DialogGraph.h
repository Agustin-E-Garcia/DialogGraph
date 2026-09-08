#pragma once

#include "CoreMinimal.h"
#include "Layout/Visibility.h"
#include "SGraphNode.h"

class UDialogAssetGraphNode;

class SGraphNode_DialogGraph : public SGraphNode
{
public:
    SLATE_BEGIN_ARGS(SGraphNode_DialogGraph){}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UEdGraphNode* inNode);

    //~ Begin SGraphNode Interface
    virtual TSharedRef<SWidget> CreateNodeContentArea() override;

protected:
    virtual void CreateOutputSideAddButton(TSharedPtr<SVerticalBox> OutputBox) override;
    virtual EVisibility IsAddPinButtonVisible() const override;
    virtual FReply OnAddPin() override;
    //~ End SGraphNode Interface

    void OnTextCommited(const FText& InText, ETextCommit::Type Type);
};
