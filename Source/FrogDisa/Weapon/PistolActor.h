// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.h"
#include "FrogDisa/InteractiveObjectsInterface.h"
#include "WeaponLogicInterface.h"
#include "PistolActor.generated.h"

UCLASS()
class FROGDISA_API APistolActor : public ABasicWeaponActor, public IWeaponLogicInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APistolActor();

	virtual bool Launch() override;
	virtual void AttachToCharacter() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* pistol_mesh;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float PistolRangeDistance = 2000;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
