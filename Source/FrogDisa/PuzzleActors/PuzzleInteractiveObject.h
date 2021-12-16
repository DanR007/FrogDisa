// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogDisa/InteractiveObject.h"
#include "PuzzleInteractiveObject.generated.h"

/**
 * 
 */
UCLASS()
class FROGDISA_API APuzzleInteractiveObject : public AInteractiveObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	FString GetName();
protected:
	UPROPERTY(EditAnywhere)
		FString Name;
};
