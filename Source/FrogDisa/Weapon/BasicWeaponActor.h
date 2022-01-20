// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.h"
#include "BasicWeaponActor.generated.h"


UCLASS()
class FROGDISA_API ABasicWeaponActor : public AActor, public AWeaponActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABasicWeaponActor();

	virtual void Create() override;
	virtual void Launch() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
