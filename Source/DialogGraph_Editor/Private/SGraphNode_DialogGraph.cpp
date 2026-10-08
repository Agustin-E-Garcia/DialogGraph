#include "SGraphNode_DialogGraph.h"
#include "DialogAssetGraphNode.h"
#include "DialogAssetGraphNode_Line.h"
#include "EdGraph/EdGraphNode.h"
#include "Math/Color.h"
#include "SGraphNode.h"
#include "SGraphPin.h"
#include "SNodePanel.h"
#include "Styling/AppStyle.h"
#include "Styling/CoreStyle.h"
#include "Styling/SlateTypes.h"
#include "Types/SlateEnums.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/SBoxPanel.h"
#include "ScopedTransaction.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"

void SGraphNode_DialogGraph::Construct(const FArguments& InArgs, UEdGraphNode* inNode)
{
    GraphNode = inNode;
    UpdateGraphNode();
}

void SGraphNode_DialogGraph::UpdateGraphNode()
{
    InputPins.Empty();
    OutputPins.Empty();

    RightNodeBox.Reset();
    LeftNodeBox.Reset();

    //             __________( )_________
    //            |                      |
    //            | TITLE _char_     Lk  |
    //            |                      |
    //            |  ~~~~~~~~~~~~~~~~~   |
    //            |  ~~~~~~~~~~~~~~~~~   |
    //            |  ~~~~~~~~~~~~~~~~~   |
    //            |                      |
    //            |__________( )_________|

    FLinearColor EdgeColor = GraphNode->GetNodeTitleColor();
    FLinearColor BodyColor = EdgeColor * 0.05f;
    BodyColor.A = 1.0f;

    this->GetOrAddSlot(ENodeZone::Center)
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Center)
        [
            SNew(SOverlay)
            +SOverlay::Slot()
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Fill)
            [
                SNew(SBorder)
                .BorderImage(FAppStyle::GetBrush("Graph.StateNode.Body"))
                .BorderBackgroundColor(EdgeColor)
                .Padding(2.0f)
                [
                    SNew(SBorder)
                    .HAlign(HAlign_Fill)
                    .VAlign(VAlign_Fill)
                    .BorderImage(FAppStyle::GetBrush("Graph.StateNode.Body"))
                    .BorderBackgroundColor(BodyColor)
                    .Padding(16.0f)
                    [
                        SNew(SBox)
                        .HAlign(HAlign_Fill)
                        .VAlign(VAlign_Fill)
                        .WidthOverride(220)
                        [
                            SNew(SVerticalBox)
                            +SVerticalBox::Slot()
                            .AutoHeight()
                            [
                                SNew(SHorizontalBox)
                                +SHorizontalBox::Slot()
                                .HAlign(HAlign_Left)
                                .VAlign(VAlign_Fill)
                                .AutoWidth()
                                [
                                    CreateIconWidget()
                                ]
                                +SHorizontalBox::Slot()
                                .HAlign(HAlign_Left)
                                .VAlign(VAlign_Fill)
                                .AutoWidth()
                                .Padding(6.0f, 0.0f, 6.0f, 0.0f)
                                [
                                    SNew(STextBlock)
                                    .Text(GraphNode->GetNodeTitle(ENodeTitleType::FullTitle))
                                    .TransformPolicy(ETextTransformPolicy::ToUpper)
                                    .Font(FCoreStyle::GetDefaultFontStyle("Bold", 11))
                                    .ColorAndOpacity(EdgeColor)
                                ]
                                +SHorizontalBox::Slot()
                                .HAlign(HAlign_Left)
                                .VAlign(VAlign_Fill)
                                .AutoWidth()
                                [
                                    SNew(STextBlock)
                                    .Text(FText::FromString("narrator"))
                                    .ColorAndOpacity(FLinearColor::White.Desaturate(0.5f))
                                ]
                            ]
                            +SVerticalBox::Slot()
                            .AutoHeight()
                            .Padding(0.0f, 6.0f, 0.0f, 6.0f)
                            [
                                CreateNodeContentArea()
                            ]
                        ]
                    ]
                ]
            ]
            +SOverlay::Slot()
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Fill)
            .Padding(0.0f, -15.0f, 0.0f, -15.0f)
            [
                SNew(SVerticalBox)
                +SVerticalBox::Slot()
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Top)
                [
                    SAssignNew(LeftNodeBox, SVerticalBox)
                ]
                +SVerticalBox::Slot()
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Bottom)
                [
                    SAssignNew(RightNodeBox, SVerticalBox)
                ]
            ]
            +SOverlay::Slot()
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Fill)
            .Padding(-8.0f, -8.0f, -8.0f, -8.0f)
            [
                CreateNodeConditionLock()
            ]
        ];

    CreatePinWidgets();
}

TSharedRef<SWidget> SGraphNode_DialogGraph::CreateNodeContentArea()
{
    UDialogAssetGraphNode_Line* GraphNode_Line = Cast<UDialogAssetGraphNode_Line>(GraphNode);
    if(!GraphNode_Line)
    {
        return SNullWidget::NullWidget;
    }

    return SNew(SMultiLineEditableTextBox)
            .Style(FAppStyle::Get(), "Graph.EditableTextBox")
            .Text(GraphNode_Line->GetDialogLine())
            .SelectAllTextWhenFocused(true)
            .OnTextCommitted(this, &SGraphNode_DialogGraph::OnTextCommited)
            .ForegroundColor(FSlateColor::UseForeground())
            .OverflowPolicy(ETextOverflowPolicy::Ellipsis)
            .AutoWrapText(false);
}

void SGraphNode_DialogGraph::OnTextCommited(const FText& InText, ETextCommit::Type Type)
{
    if(UDialogAssetGraphNode_Line* GraphNode_Line = Cast<UDialogAssetGraphNode_Line>(GraphNode))
    {
        const FScopedTransaction Transaction(FText::FromString("Change Choice Pin Value"));
        GraphNode_Line->Modify();

        GraphNode_Line->SetDialogLine(InText);
    }
}

TSharedRef<SWidget> SGraphNode_DialogGraph::CreateNodeConditionLock()
{
    UDialogAssetGraphNode* Node = Cast<UDialogAssetGraphNode>(GraphNode);

    if(Node && Node->CanUserAddCondition() && Node->HasConditions())
    {
        return SNew(SBox)
                .HAlign(HAlign_Right)
                .VAlign(VAlign_Top)
                [
                    SNew(SImage)
                    .Image(FAppStyle::GetBrush("Sequencer.LockSequence"))
                    .ColorAndOpacity(FLinearColor::Red)
                ];
    }

    return SNullWidget::NullWidget;
}
