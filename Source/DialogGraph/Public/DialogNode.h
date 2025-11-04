#pragma once

#include <CoreMinimal.h>
#include <DialogNode.generated.h>

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

UENUM()
enum NodeType
{
    Start,
    End,
    Line,
    Choice,
};

USTRUCT(BlueprintType)
struct FDialogNode
{
    GENERATED_BODY()

    FDialogNode() {}
    FDialogNode(NodeType type, int id) : ID(id), NodeType(type) {}

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int ID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TEnumAsByte<NodeType> NodeType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString DialogText;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<int> NextIDs;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FEditorData editorData;
};
