#pragma once

#include <CoreMinimal.h>
#include <DialogNode.generated.h>

USTRUCT(BlueprintType)
struct FPinInfo
{
    GENERATED_BODY()

    FPinInfo() {}
    FPinInfo(int id, FString title) : NextID(id), Title(title) {}

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int NextID;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) FString Title;
};

UENUM(BlueprintType)
enum class EDialogNodeType : uint8
{
    DEFAULT UMETA(Hidden),
    Start UMETA(Hidden),
    Line,
    Choice,
};

USTRUCT(BlueprintType)
struct FDialogNode
{
    GENERATED_BODY()

    FDialogNode() { }
    FDialogNode(EDialogNodeType type, int id) : ID(id), NodeType(type) {}

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int ID;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) EDialogNodeType NodeType;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TArray<FPinInfo> NextIDs;
};
