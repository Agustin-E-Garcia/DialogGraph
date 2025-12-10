#include "EdGraph/EdGraphNode.h"
#include <EditorGraph/DialogGraphSchema.h>
#include <EditorGraph/GraphNodes/DialogGraphNode_Choice.h>
#include <EditorGraph/GraphNodes/DialogGraphNode_Line.h>
#include <EditorGraph/GraphNodes/DialogGraphNode_Start.h>
#include <EditorGraph/GraphNodes/DialogGraphNode_Transition.h>
#include <EditorGraph/DialogGraphStyle/DialogConnectionDrawingPolicy.h>
#include <DialogNode.h>
#include <EdGraph/EdGraph.h>
#include <EdGraph/EdGraphPin.h>

// ===== UDilaogGraphSchema functions

void UDialogGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& contextMenuBuilder) const
{
    TSharedPtr<FNewChoiceNodeAction> newChoiceAction(new FNewChoiceNodeAction());
    TSharedPtr<FNewLineNodeAction> newLineAction(new FNewLineNodeAction());

    contextMenuBuilder.AddAction(newChoiceAction);
    contextMenuBuilder.AddAction(newLineAction);
}

const FPinConnectionResponse UDialogGraphSchema::CanCreateConnection(const UEdGraphPin* a, const UEdGraphPin* b) const
{
    if(a == nullptr || b == nullptr) return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT(""));
    if(a->Direction == b->Direction) return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Inputs can only connect to outputs"));
    return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT(""));
}

bool UDialogGraphSchema::CreateAutomaticConversionNodeAndConnections(UEdGraphPin* PinA, UEdGraphPin* PinB) const { return true; }

bool UDialogGraphSchema::TryCreateConnection(UEdGraphPin* a, UEdGraphPin* b) const
{
    if(Cast<UDialogGraphNode_Choice>(a->GetOwningNode()) == nullptr) return Super::TryCreateConnection(a, b);

    const FPinConnectionResponse Response = CanCreateConnection(a, b);
    bool bModified = false;

    switch (Response.Response)
    {
    case CONNECT_RESPONSE_MAKE:
        a->MakeLinkTo(b);
        bModified = CreateConnectionWithTransition(a, b);
        break;

    case CONNECT_RESPONSE_BREAK_OTHERS_A:
        a->BreakAllPinLinks(true);
        bModified = CreateConnectionWithTransition(a, b);
        break;

    case CONNECT_RESPONSE_BREAK_OTHERS_B:
        b->BreakAllPinLinks(true);
        bModified = CreateConnectionWithTransition(a, b);
        break;

    case CONNECT_RESPONSE_BREAK_OTHERS_AB:
        a->BreakAllPinLinks(true);
        b->BreakAllPinLinks(true);
        bModified = CreateConnectionWithTransition(a, b);
        break;

    case CONNECT_RESPONSE_DISALLOW:
    default:
        break;
    }

#if WITH_EDITOR
    if (bModified)
    {
        a->GetOwningNode()->PinConnectionListChanged(a);
        b->GetOwningNode()->PinConnectionListChanged(b);
    }
#endif  //#if WITH_EDITOR

    return bModified;
}

bool UDialogGraphSchema::CreateConnectionWithTransition(UEdGraphPin* a, UEdGraphPin* b) const
{
    UEdGraph* parentGraph = a->GetOwningNode()->GetGraph();

    UEdGraphNode* nodeA = a->GetOwningNode();
    UEdGraphNode* nodeB = b->GetOwningNode();

    FVector2f Location = (FVector2f(nodeA->NodePosX, nodeA->NodePosY) + FVector2f(nodeB->NodePosX, nodeB->NodePosY)) * 0.5f;

    UDialogGraphNode_Transition* node = NewObject<UDialogGraphNode_Transition>(parentGraph);
    if (node == nullptr) return false;

    node->NodePosX = Location.X;
    node->NodePosY = Location.Y;

    UEdGraphPin* inputPin = node->GetPinAt(0);
    UEdGraphPin* outputPin = node->GetPinAt(1);

    a->MakeLinkTo(inputPin);
    outputPin->MakeLinkTo(b);

    parentGraph->Modify();
    parentGraph->AddNode(node, true, true);

    return true;
}

void UDialogGraphSchema::BreakConnectionWithTransition(UEdGraphPin* target) const
{
    UEdGraph* parentGraph = target->GetOwningNode()->GetGraph();
    if(target->GetOwningNode()->IsA(UDialogGraphNode_Transition::StaticClass())) parentGraph->RemoveNode(target->GetOwningNode());

    for(int i = 0; i < target->LinkedTo.Num(); i++)
    {
        if(target->LinkedTo[i]->GetOwningNode()->IsA(UDialogGraphNode_Transition::StaticClass()))
            parentGraph->RemoveNode(target->LinkedTo[i]->GetOwningNode());
    }
}

void UDialogGraphSchema::BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotifcation) const
{
    UE_LOG(LogTemp, Warning, TEXT("UDialogGraphSchema::BreakPinLinks()"));

    BreakConnectionWithTransition(&TargetPin);

    UEdGraphSchema::BreakPinLinks(TargetPin, bSendsNodeNotifcation);
}

FConnectionDrawingPolicy* UDialogGraphSchema::CreateConnectionDrawingPolicy(
    int32 InBackLayerID,
    int32 InFrontLayerID,
    float InZoomFactor,
    const FSlateRect& InClippingRect,
    FSlateWindowElementList& InDrawElements,
    UEdGraph* InGraphObj) const
{
    return new FDialogConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements, InGraphObj);
}


// ===== NodeActions functions =====

UEdGraphNode* FNewChoiceNodeAction::PerformAction(UEdGraph* parentGraph, UEdGraphPin* fromPin, const FVector2D location, bool bSelectNewNode)
{
    UDialogGraphNode_Base* result = NewObject<UDialogGraphNode_Choice>(parentGraph);
    FEditorData editorData(1, FVector2f(location.X, location.Y), "");
    result->SetupNode(fromPin, &editorData);

    parentGraph->Modify();
    parentGraph->AddNode(result, true, true);

    return result;
}

UEdGraphNode* FNewLineNodeAction::PerformAction(UEdGraph* parentGraph, UEdGraphPin* fromPin, const FVector2D location, bool bSelectNewNode)
{
    UDialogGraphNode_Base* result = NewObject<UDialogGraphNode_Line>(parentGraph);
    FEditorData editorData(1, FVector2f(location.X, location.Y), "");
    result->SetupNode(fromPin, &editorData);

    parentGraph->Modify();
    parentGraph->AddNode(result, true, true);

    return result;
}
