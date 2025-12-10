#pragma once

#include <EdGraphUtilities.h>
#include <SGraphNode.h>
#include <SGraphPin.h>
#include <Widgets/Layout/SBox.h>
#include <SGraphPanel.h>
#include <EditorGraph/GraphNodes/DialogGraphNode_Transition.h>
#include <Widgets/Images/SImage.h>

class SDialogTransitionNode : public SGraphNode
{
public:
    SLATE_BEGIN_ARGS(SDialogTransitionNode) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UEdGraphNode* inNode) 
    {
        GraphNode = inNode;
        UpdateGraphNode();
    }

    virtual void UpdateGraphNode() override
    {
        //TODO: Make the visuals of the transition node
        InputPins.Empty();
        OutputPins.Empty();
        RightNodeBox.Reset();
        LeftNodeBox.Reset();

        TSharedRef<SImage> DirectionImage = SNew(SImage)
        .Image(this, &SDialogTransitionNode::GetTransitionIconImage)
        .ColorAndOpacity(FStyleColors::Background);

        DirectionImage->SetRenderTransform(MakeAttributeLambda([this]()-> TOptional<FSlateRenderTransform>
        {
            UDialogGraphNode_Transition* TransNode = CastChecked<UDialogGraphNode_Transition>(GraphNode);
            return FSlateRenderTransform(FQuat2D(TransNode->CachedRotation));
        }));
        DirectionImage->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));

        this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
        this->GetOrAddSlot(ENodeZone::Center)
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Fill)
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot()
            [
                SNew(SOverlay)
                + SOverlay::Slot()
                .Padding(4.0f)
                [
                    SNew(SImage)
                    .Image(FAppStyle::GetBrush("Graph.Node.Body"))
                    .ColorAndOpacity(FStyleColors::Background)
                ]
                + SOverlay::Slot()
                [
                    SNew(SBox)
                    .VAlign(VAlign_Center)
                    .HAlign(HAlign_Center)
                    .MinDesiredHeight(40.f)
                    .MinDesiredWidth(40.0f)
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString(TEXT("\u2713")))
                        .Justification(ETextJustify::Center)
                    ]
                ]
            ]
        ];
    }

    virtual bool RequiresSecondPassLayout() const override { return true; }
    virtual void PerformSecondPassLayout(const TMap< UObject*, TSharedRef<SNode> >& NodeToWidgetLookup) const override
    {
        // Here we update the position of the transition node
    }

    virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
    {
        if(GraphNode->Pins[0] != NULL) GetOwnerPanel()->AddPinToHoverSet(GraphNode->Pins[0]);
        SGraphNode::OnMouseEnter(MyGeometry, MouseEvent);
    }

    virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override
    {
        if(GraphNode->Pins[0] != NULL) GetOwnerPanel()->RemovePinFromHoverSet(GraphNode->Pins[0]);
        SGraphNode::OnMouseLeave(MouseEvent);
    }

    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
    {
        return SGraphNode::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId + 100, InWidgetStyle, bParentEnabled);
    }

private:
    const FSlateBrush* GetTransitionIconImage() const { return FAppStyle::GetBrush("Graph.AnimTransitionNode.Icon"); }
};

struct FDialogTransitionNodeFactory : public FGraphPanelNodeFactory
{
public:
    virtual ~FDialogTransitionNodeFactory () {}
    virtual TSharedPtr<SGraphNode> CreateNode(UEdGraphNode* node) const override 
    {
        if(Cast<UDialogGraphNode_Transition>(node) == nullptr) return nullptr;
        return SNew(SDialogTransitionNode, node);
    }
};
