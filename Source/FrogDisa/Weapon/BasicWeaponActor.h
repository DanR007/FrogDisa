// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponLogicInterface.h"
#include "BasicWeaponActor.generated.h"


UCLASS()
class FROGDISA_API ABasicWeaponActor : public AActor, public IWeaponLogicInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABasicWeaponActor();

	UFUNCTION(BlueprintCallable)
		float GetDamage() { return damage; }
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float damage = 10;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
