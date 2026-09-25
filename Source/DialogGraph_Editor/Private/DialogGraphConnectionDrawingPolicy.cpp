#include "DialogGraphConnectionDrawingPolicy.h"
#include "ConnectionDrawingPolicy.h"
#include "EdGraph/EdGraphNode.h"
#include "Layout/PaintGeometry.h"
#include "Math/Color.h"
#include "Rendering/DrawElementTypes.h"

FDialogGraphConnectionDrawingPolicy::FDialogGraphConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj)
    : FConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements), GraphObj(InGraphObj)
{
}

void FDialogGraphConnectionDrawingPolicy::DrawSplineWithArrow(const FGeometry& StartGeom, const FGeometry& EndGeom, const FConnectionParams& Params)
{
    FVector2f StartPoint = FGeometryHelper::CenterOf(StartGeom);
    FVector2f EndPoint = FGeometryHelper::CenterOf(EndGeom);

    DrawSplineWithArrow(StartPoint, EndPoint, Params);
}
void FDialogGraphConnectionDrawingPolicy::DrawSplineWithArrow(const FVector2f& StartPoint, const FVector2f& EndPoint, const FConnectionParams& Params)
{
    DrawConnection(WireLayerID, StartPoint, EndPoint, Params);
}

void FDialogGraphConnectionDrawingPolicy::DrawPreviewConnector(const FGeometry& PinGeometry, const FVector2f& StartPoint, const FVector2f& EndPoint, UEdGraphPin* Pin)
{
    FConnectionParams Params;
    DetermineWiringStyle(Pin, nullptr, Params);

    FVector2f CustomStartPoint = Pin->Direction == EGPD_Input ? StartPoint : FGeometryHelper::CenterOf(PinGeometry);
    FVector2f CustomEndPoint   = Pin->Direction == EGPD_Input ? FGeometryHelper::CenterOf(PinGeometry) : EndPoint;

    DrawSplineWithArrow(CustomStartPoint, CustomEndPoint, Params);
}

void FDialogGraphConnectionDrawingPolicy::DrawConnection(int LayerID, const FVector2f& Start, const FVector2f& End, const FConnectionParams& Params)
{
    FVector2f StartStub = Start + FVector2f(0.0f, 20.0f);
    FVector2f EndStub = End - FVector2f(0.0f, 20.0f);

    TArray<FVector2f> Points;
    Points.Add(Start);
    Points.Add(StartStub);

    // Calculate and add all the intermediate points we need
    bool bIsEndBelow = EndStub.Y > StartStub.Y;
    FVector2f Distance = EndStub - StartStub;

    if(bIsEndBelow)
    {
        Points.Add(FVector2f(StartStub.X, StartStub.Y + (Distance.Y * 0.5f)));
        Points.Add(FVector2f(StartStub.X + Distance.X, StartStub.Y + (Distance.Y * 0.5f)));
    }
    else
    {
        Points.Add(FVector2f(StartStub.X + (Distance.X * 0.5f), StartStub.Y));
        Points.Add(FVector2f(StartStub.X + (Distance.X * 0.5f), StartStub.Y + Distance.Y));
    }

    Points.Add(EndStub);
    Points.Add(End);

    FSlateDrawElement::MakeLines(
            DrawElementsList,
            WireLayerID,
            FPaintGeometry(),
            Points,
            ESlateDrawEffect::None,
            FLinearColor::White,
            true,
            Settings->SliceLineThickness);
}
