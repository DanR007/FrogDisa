// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PointOfPatrol.generated.h"

UCLASS()
class FROGDISA_API APointOfPatrol : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APointOfPatrol();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WaitTime;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* Collider;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
