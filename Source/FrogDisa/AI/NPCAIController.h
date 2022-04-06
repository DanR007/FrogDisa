// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "FrogDisa/DefaultVariables.h"
#include "NPCAIController.generated.h"

/**
 * 
 */
UCLASS()
class FROGDISA_API ANPCAIController : public AAIController
{
	GENERATED_BODY()

	ANPCAIController();

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	

protected:	
	UFUNCTION()
	void NewPerception(AActor* NewActorPerception, FAIStimulus stimulus);
};
