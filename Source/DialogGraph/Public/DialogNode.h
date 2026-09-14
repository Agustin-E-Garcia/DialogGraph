#pragma once

#include "CoreMinimal.h"
#include "StructUtils/PropertyBag.h"
#include "DialogNode.generated.h"

USTRUCT(BlueprintType)
struct FPinInfo
{
    GENERATED_USTRUCT_BODY()

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
    Task,
};

USTRUCT(BlueprintType)
struct FDialogNode
{
    GENERATED_USTRUCT_BODY()

    FDialogNode() {}
    FDialogNode(EDialogNodeType type, int id) : ID(id), NodeType(type) {}

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    EDialogNodeType NodeType;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<FPinInfo> NextIDs;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UClass> FunctionClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FName FunctionName;

    UPROPERTY(VisibleAnywhere)
    FInstancedPropertyBag FunctionProperties;
};
