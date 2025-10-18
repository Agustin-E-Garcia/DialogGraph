#pragma once

#include <CoreMinimal.h>
#include <Factories/Factory.h>
#include <DialogAssetFactory.generated.h>

UCLASS()
class UDialogAssetFactory : public UFactory
{
    GENERATED_BODY()

public:
    UDialogAssetFactory(const FObjectInitializer& objectInitializer);

    virtual UObject* FactoryCreateNew(UClass* uclass, UObject* inParent, FName name, EObjectFlags flags, UObject* context, FFeedbackContext* warn);
    virtual bool CanCreateNew() const override;
};
