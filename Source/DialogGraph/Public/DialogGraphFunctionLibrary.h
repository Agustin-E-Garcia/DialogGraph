#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DialogGraphFunctionLibrary.generated.h"

class UDialogAsset;

UCLASS(Blueprintable, Abstract)
class DIALOGGRAPH_API UDialogGraphFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
};
