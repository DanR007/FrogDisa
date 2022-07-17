// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicWeaponActor.h"
#include "FrogDisa/InteractiveClass.h"
#include "Components/SphereComponent.h"
#include "MineActor.generated.h"

/**
 * 
 */
UCLASS()
class FROGDISA_API AMineActor : public ABasicWeaponActor, public IInteractiveObjectsInterface, public InteractiveClass
{
	GENERATED_BODY()
public:



	AMineActor();
	virtual bool Launch() override;
	virtual void AttachToCharacter() override;
	virtual void Interact() override;
	virtual void ChangeOutlines_Implementation(bool isOutline) override { UE_LOG(LogTemp, Warning, TEXT("C++")) }

	UFUNCTION(BlueprintCallable)
		bool GetIsReplace() { return isReplace; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* mainMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USphereComponent* ExplosionCollision;

	FCollisionQueryParams queryParams;

	UFUNCTION(BlueprintCallable)
		virtual void Explosion();

protected:
	const EInteractionTypes interactive_type = EInteractionTypes::EIT_Take;
	UFUNCTION(BlueprintNativeEvent)
		void CallExplosionParticle();
private:

	int Time_Before_Explosion;

	bool isReplace = false;
};
