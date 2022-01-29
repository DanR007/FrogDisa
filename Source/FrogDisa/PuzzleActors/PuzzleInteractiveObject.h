#pragma once

#include "CoreMinimal.h"
#include "FrogDisa/CarriedObject.h"
#include "PuzzleInteractiveObject.generated.h"

/**
 * 
 */
UCLASS()
class FROGDISA_API APuzzleCarriedObject : public ACarriedObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	FString GetName();
protected:
	UPROPERTY(EditAnywhere)
		FString Name;
};
