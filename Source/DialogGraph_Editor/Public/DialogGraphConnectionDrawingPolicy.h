#pragma once

#include "CoreMinimal.h"
#include "ConnectionDrawingPolicy.h"

struct FGeometry;
class FSlateRect;
class FSlateWindowElementList;
class UEdGraph;
class UEdGraphPin;

class FDialogGraphConnectionDrawingPolicy : public FConnectionDrawingPolicy
{
protected:
    UEdGraph* GraphObj;

public:
    FDialogGraphConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj);

    //~ Begin FConnectionDrawingPolicy Interface
    virtual void DrawSplineWithArrow(const FGeometry& StartGeom, const FGeometry& EndGeom, const FConnectionParams& Params) override;
    virtual void DrawSplineWithArrow(const FVector2f& StartPoint, const FVector2f& EndPoint, const FConnectionParams& Params) override;
    virtual void DrawPreviewConnector(const FGeometry& PinGeometry, const FVector2f& StartPoint, const FVector2f& EndPoint, UEdGraphPin* Pin) override;
    virtual void DrawConnection(int LayerID, const FVector2f& Start, const FVector2f& End, const FConnectionParams& Params) override;
    //~ End FConnectionDrawingPolicy Interface
};
