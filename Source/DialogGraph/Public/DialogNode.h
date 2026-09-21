#pragma once

#include "CoreMinimal.h"
#include "StructUtils/PropertyBag.h"
#include "DialogNode.generated.h"

USTRUCT(BlueprintType)
struct FBindedFunctionData
{
    GENERATED_USTRUCT_BODY()

    FBindedFunctionData() {}
    FBindedFunctionData(TObjectPtr<UClass> InClass, FName InName, FInstancedPropertyBag InParameters)
        : Class(InClass), Name(InName), Parameters(InParameters) {}

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UClass> Class;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FName Name;

    UPROPERTY(EditAnywhere)
    FInstancedPropertyBag Parameters;
};

USTRUCT(BlueprintType)
struct FPinInfo
{
    GENERATED_USTRUCT_BODY()

    FPinInfo() {}
    FPinInfo(int id, FString title) : NextID(id), Title(title) {}

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int NextID;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) FString Title;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FBindedFunctionData> ConditionsData;
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

    UPROPERTY(EditAnywhere)
    FBindedFunctionData BindedTaskFunction;
};
