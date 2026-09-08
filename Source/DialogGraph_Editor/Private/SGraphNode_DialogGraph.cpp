#include "SGraphNode_DialogGraph.h"
#include "DialogAssetGraphNode_Choice.h"
#include "DialogAssetGraphNode_Line.h"
#include "EdGraph/EdGraphNode.h"
#include "Layout/Visibility.h"
#include "SGraphPin.h"
#include "Types/SlateEnums.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/SBoxPanel.h"
#include "ScopedTransaction.h"
#include "EdGraph/EdGraph.h"

void SGraphNode_DialogGraph::Construct(const FArguments& InArgs, UEdGraphNode* inNode)
{
    GraphNode = inNode;
    UpdateGraphNode();
}

TSharedRef<SWidget> SGraphNode_DialogGraph::CreateNodeContentArea()
{
    UDialogAssetGraphNode_Line* GraphNode_Line = Cast<UDialogAssetGraphNode_Line>(GraphNode);
    if(!GraphNode_Line)
    {
        return SGraphNode::CreateNodeContentArea();
    }

    return SNew(SBorder)
        .BorderImage( FAppStyle::GetBrush("NoBorder") )
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Fill)
        .Padding( FMargin(0,3) )
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
            .VAlign(VAlign_Center)
            .MinWidth(100.0f)
            [
                // CENTER
                SNew(SMultiLineEditableTextBox)
                .Style(FAppStyle::Get(), "Graph.EditableTextBox")
                .Text(GraphNode_Line->GetDialogLine())
                .SelectAllTextWhenFocused(true)
                .OnTextCommitted(this, &SGraphNode_DialogGraph::OnTextCommited)
                .ForegroundColor(FSlateColor::UseForeground())
                .WrapTextAt(400.0f)
                .ModiferKeyForNewLine(EModifierKey::Shift)
            ]
            +SHorizontalBox::Slot()
            .AutoWidth()
            .HAlign(HAlign_Right)
            [
                // RIGHT
                SAssignNew(RightNodeBox, SVerticalBox)
            ]
        ];
}

void SGraphNode_DialogGraph::CreateOutputSideAddButton(TSharedPtr<SVerticalBox> OutputBox)
{
    TSharedRef<SWidget> AddPinButton = AddPinButtonContent(FText::FromString("Add Pin"), FText::FromString("Add new pin"));

    OutputBox->AddSlot()
    .AutoHeight()
    .VAlign(VAlign_Center)
    [
        AddPinButton
    ];
}

EVisibility SGraphNode_DialogGraph::IsAddPinButtonVisible() const
{
    UDialogAssetGraphNode_Choice* GraphNode_Choice = Cast<UDialogAssetGraphNode_Choice>(GraphNode);
    if(GraphNode_Choice) return EVisibility::Visible;

    return EVisibility::Collapsed;
}

FReply SGraphNode_DialogGraph::OnAddPin()
{
    UDialogAssetGraphNode_Choice* GraphNode_Choice = CastChecked<UDialogAssetGraphNode_Choice>(GraphNode);

    const FScopedTransaction Transaction(FText::FromString("Add Option Pin"));
    GraphNode_Choice->Modify();

    GraphNode_Choice->AddPin();
    UpdateGraphNode();
    GraphNode_Choice->GetGraph()->NotifyNodeChanged(GraphNode_Choice);

    return FReply::Handled();
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
