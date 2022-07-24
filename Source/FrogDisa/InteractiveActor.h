// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/EProjectiles.h"

#include "CarriedObjectLogicInterface.h"
#include "InteractiveActor.generated.h"

UCLASS()
class FROGDISA_API AInteractiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveActor();

	UPROPERTY(EditDefaultsOnly)
		EInteractionTypes interactive_type;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
