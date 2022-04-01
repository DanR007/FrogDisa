// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponActor.h"
#include "FrogDisa/InteractiveObjectsInterface.h"
#include "WeaponLogicInterface.h"
#include "Components/SphereComponent.h"

#include "MineActor.generated.h"

/**
 * 
 */
UCLASS()
class FROGDISA_API AMineActor : public AActor, public IWeaponLogicInterface, public IInteractiveObjectsInterface
{
	GENERATED_BODY()
public:

	AMineActor();
	virtual void Launch() override;
	virtual void AttachToCharacter() override;
	virtual void Interact() override;
	virtual void ChangeOutlines_Implementation(bool isOutline) override { UE_LOG(LogTemp, Warning, TEXT("C++")) }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* mainMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USphereComponent* ExplosionCollision;

	FCollisionQueryParams queryParams;

	UFUNCTION(BlueprintCallable)
		virtual void Explosion();
private:

	int Time_Before_Explosion;


};
