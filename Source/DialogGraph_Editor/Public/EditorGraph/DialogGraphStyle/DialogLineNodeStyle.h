#pragma once

#include "Templates/Casts.h"
#include "Types/SlateEnums.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SBoxPanel.h"
#include <EdGraphUtilities.h>
#include <KismetPins/SGraphPinColor.h>
#include <EdGraph/EdGraphPin.h>
#include <Widgets/Text/SMultiLineEditableText.h>
#include <EditorGraph/GraphNodes/DialogGraphNode_Line.h>
#include <GraphEditorSettings.h>

class SDialogLineNode : public SGraphNode
{
public:
    SLATE_BEGIN_ARGS(SDialogLineNode) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UEdGraphNode* inNode) 
    {
        GraphNode = inNode;
        SGraphNode::UpdateGraphNode();
    }

    virtual TSharedRef<SWidget> CreateNodeContentArea() override
    {
        // NODE CONTENT AREA
        return SNew(SBorder)
            .BorderImage( FAppStyle::GetBrush("NoBorder") )
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Fill)
            .Padding( FMargin(0,3) )
            [
                SNew(SVerticalBox)
                + SVerticalBox::Slot()
                .VAlign(VAlign_Top)
                .AutoHeight()
                [
                    SNew(SHorizontalBox)
                    +SHorizontalBox::Slot()
                    .HAlign(HAlign_Left)
                    .FillWidth(1.0f)
                    [
                        // LEFT
                        SAssignNew(LeftNodeBox, SVerticalBox)
                    ]
                    +SHorizontalBox::Slot()
                    .AutoWidth()
                    .HAlign(HAlign_Right)
                    [
                        // RIGHT
                        SAssignNew(RightNodeBox, SVerticalBox)
                    ]
                ]
                + SVerticalBox::Slot()
                .VAlign(VAlign_Bottom)
                .AutoHeight()
                .Padding(Settings->GetInputPinPadding())
                [
                    SNew(SBox)
                    .MinDesiredWidth(18.0f)
                    .MaxDesiredHeight(200.0f)
                    [
                        SNew(SMultiLineEditableText)
                        .Text(this, &SDialogLineNode::GetNodeLine)
                        .OnTextCommitted(this, &SDialogLineNode::OnLineTextCommited)
                        .SelectAllTextWhenFocused(true)
                        .WrapTextAt(400.0f)
                    ]
                ]
            ];
    }

private:
    void OnLineTextCommited(const FText& NewText, ETextCommit::Type CommitType) 
    {
        Cast<UDialogGraphNode_Line>(GraphNode)->SetDialogLine(NewText);
    }

    FText GetNodeLine() const 
    {
        FText result = Cast<UDialogGraphNode_Line>(GraphNode)->GetDialogLine();

        if(result.IsEmpty()) result = FText::FromString(TEXT("Enter dialog line..."));
        return result;
    }

    TSharedPtr<SMultiLineEditableText> _LineTextBox;
};

struct FDialogLineNodeFactory : public FGraphPanelNodeFactory
{
public:
    virtual ~FDialogLineNodeFactory() {}
    virtual TSharedPtr<SGraphNode> CreateNode(UEdGraphNode* node) const override 
    {
        if(Cast<UDialogGraphNode_Line>(node) == nullptr) return nullptr;
        return SNew(SDialogLineNode, node);
    }
};
