#pragma once

#include <CoreMinimal.h>
#include <DialogNode.generated.h>

#if WITH_EDITORONLY_DATA
USTRUCT(BlueprintType)
struct FEditorData
{
    GENERATED_BODY()

    FEditorData() {}
    FEditorData(int outputPinCount, FVector2f position, FString comment) : OutputPinCount(outputPinCount), Position(position), Comment(comment) {}

    UPROPERTY(VisibleAnywhere)
    int OutputPinCount = 1;
    UPROPERTY(VisibleAnywhere)
    FVector2f Position = FVector2f(0.0f, 0.0f);
    UPROPERTY(VisibleAnywhere)
    FString Comment = "";
};
#endif

UENUM()
enum NodeType
{
    Start,
    End,
    Line,
    Choice,
};

USTRUCT(BlueprintType)
struct GRAPHEDITOR_API FDialogNode
{
    GENERATED_BODY()

    FDialogNode() {}
    FDialogNode(NodeType type, int id) : ID(id), NodeType(type) {}

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TEnumAsByte<NodeType> NodeType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString DialogText;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<int> NextIDs;

#if WITH_EDITORONLY_DATA
    UPROPERTY(VisibleAnywhere)
    FEditorData editorData;
#endif
};
