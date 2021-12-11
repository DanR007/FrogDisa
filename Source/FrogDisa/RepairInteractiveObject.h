// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveObject.h"
#include "RepairInteractiveObject.generated.h"

/**
 * 
 */
UCLASS()
class FROGDISA_API ARepairInteractiveObject : public AInteractiveObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;

};
