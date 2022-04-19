// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FrogDisa/InteractiveObjectsInterface.h"
#include "WeaponLogicInterface.h"

#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"

#include "GrenadeActor.generated.h"

UCLASS()
class FROGDISA_API AGrenadeActor : public AActor, public IWeaponLogicInterface, public IInteractiveObjectsInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenadeActor();

	virtual bool Launch() override;
	virtual void AttachToCharacter() override;
	virtual void Interact() override;
	virtual void ChangeOutlines_Implementation(bool isOutline) override { UE_LOG(LogTemp, Warning, TEXT("C++")) }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* mainMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USphereComponent* ExplosionCollision;

	UFUNCTION(BlueprintCallable)
		virtual void Explosion();


protected:

	UFUNCTION(BlueprintNativeEvent)
		void CallExplosionParticle();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle ExplosionTimer;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
