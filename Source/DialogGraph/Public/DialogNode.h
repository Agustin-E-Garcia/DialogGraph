#pragma once

#include <CoreMinimal.h>
#include <DialogNode.generated.h>

USTRUCT(BlueprintType)
struct FDialogNode
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int ID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString DialogText;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int NextDialogID;
};
