#pragma once

#include <CoreMinimal.h>
#include <DialogNode.generated.h>

USTRUCT(BlueprintType)
struct FEditorData
{
    GENERATED_BODY()

    FEditorData() {}
    FEditorData(FVector2f position, FString comment) : Position(position), Comment(comment) {}

    UPROPERTY(VisibleAnywhere) FVector2f Position = FVector2f(0.0f, 0.0f);
    UPROPERTY(VisibleAnywhere) FString Comment = "";
};

USTRUCT(BlueprintType)
struct FPinInfo
{
    GENERATED_BODY()

    FPinInfo() {}
    FPinInfo(int id, FString title) : NextID(id), Title(title) {}

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int NextID;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) FString Title;
};

UENUM()
enum NodeType
{
    DEFAULT,
    Line,
    Choice,
};

USTRUCT(BlueprintType)
struct FDialogNode
{
    GENERATED_BODY()

    FDialogNode() { }
    FDialogNode(NodeType type, int id) : ID(id), NodeType(type) {}

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int ID;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TEnumAsByte<NodeType> NodeType;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TArray<FPinInfo> NextIDs;

    // TODO flag with editor only so it's not compiled into the game
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) FEditorData editorData;
};
