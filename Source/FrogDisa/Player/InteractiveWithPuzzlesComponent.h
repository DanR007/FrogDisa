// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractiveWithPuzzlesComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FROGDISA_API UInteractiveWithPuzzlesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractiveWithPuzzlesComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	void ActionWithPuzzleActor();

	bool IsPuzzleTypeWithoutSpecialView(AActor* puzzleActor);
};
