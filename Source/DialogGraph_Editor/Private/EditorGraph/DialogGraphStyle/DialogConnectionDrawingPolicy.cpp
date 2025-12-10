#include <EditorGraph/DialogGraphStyle/DialogConnectionDrawingPolicy.h>
#include <EditorGraph/GraphNodes/DialogGraphNode_Transition.h>
#include <EditorGraph/DialogGraphStyle/DialogPinStyle.h>
#include <EdGraph/EdGraph.h>

FDialogConnectionDrawingPolicy::FDialogConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor,
        const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj)
        : FConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements) {}

void FDialogConnectionDrawingPolicy::DrawConnection(int32 LayerId, const FVector2f& Start, const FVector2f& End, const FConnectionParams& Params)
{
    FConnectionDrawingPolicy::DrawConnection(LayerId, Start, End, Params);
}

void FDialogConnectionDrawingPolicy::DetermineLinkGeometry(FArrangedChildren& ArrangedNodes, TSharedRef<SWidget>& OutputPinWidget, UEdGraphPin* OutputPin, UEdGraphPin* InputPin,
    /*out*/ FArrangedWidget*& StartWidgetGeometry, /*out*/ FArrangedWidget*& EndWidgetGeometry)
{
    if(OutputPin->GetOwningNode()->IsA(UDialogGraphNode_Transition::StaticClass())) return;

    if(InputPin->GetOwningNode()->IsA(UDialogGraphNode_Transition::StaticClass()))
    {
        UEdGraphNode* node = InputPin->GetOwningNode();
        if(node->Pins.Num() <= 1) return;
        if(node->Pins[1]->LinkedTo.Num() <= 0) return;

        InputPin = node->Pins[1]->LinkedTo[0];
    }

    FConnectionDrawingPolicy::DetermineLinkGeometry(ArrangedNodes, OutputPinWidget, OutputPin, InputPin, StartWidgetGeometry, EndWidgetGeometry);
}


void FDialogConnectionDrawingPolicy::DetermineWiringStyle(UEdGraphPin* OutputPin, UEdGraphPin* InputPin, /*inout*/ FConnectionParams& Params)
{
    Params.AssociatedPin1 = OutputPin;
    Params.AssociatedPin2 = InputPin;
    Params.WireThickness = 1.5f;
    Params.WireColor = FStyleColors::White.GetSpecifiedColor();

    if (InputPin)
    {
        if (UDialogGraphNode_Transition* TransNode = Cast<UDialogGraphNode_Transition>(InputPin->GetOwningNode()))
        {
            if(HoveredPins.Contains(InputPin)) Params.WireColor = FStyleColors::AccentOrange.GetSpecifiedColor().Desaturate(0.25f);
        }
    }
}
