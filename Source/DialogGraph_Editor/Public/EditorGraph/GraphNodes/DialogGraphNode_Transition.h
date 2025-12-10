#pragma once

#include <CoreMinimal.h>
#include <EdGraph/EdGraph.h>
#include <DialogGraphNode_Transition.generated.h>

class UToolMenu;
class UGraphNodeContextMenuContext;
struct FToolMenuSection;
struct FEditorData;

UCLASS()
class UDialogGraphNode_Transition : public UEdGraphNode
{
    GENERATED_BODY()

public:
    UDialogGraphNode_Transition();
    virtual bool CanUserDeleteNode() const override { return false; }

    UPROPERTY()
    FVector2D CachedRotation;
};
