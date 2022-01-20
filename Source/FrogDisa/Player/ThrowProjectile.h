// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"

#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

#include "FrogDisa/Weapon/WeaponActor.h"
#include "FrogDisa/Weapon/BasicWeaponActor.h"

#include "ThrowProjectile.generated.h"

UCLASS()
class FROGDISA_API AThrowProjectile : public ABasicWeaponActor
{
	GENERATED_BODY()

public:

	void Create() override;
	// Sets default values for this actor's properties
	AThrowProjectile();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
		void ReturnToCharacter();
	UFUNCTION(BlueprintCallable)
		void Launch() override;

	//virtual void OnActorBeginOverlap() override;
	void AttachToPlayerCharacter(AActor* Character);
	bool GetInAirState();
	AActor* GetOwnerPlayer();
	UFUNCTION(BlueprintNativeEvent)
		void OnOverlap(AActor* OverlappedActor, AActor* OtherActor);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* ProjectileMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCapsuleComponent* ProjectileColliderD;
	UFUNCTION(BlueprintCallable)
		void ReturnStart();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool projectileIsReturning;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool isLaunched;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool inAir;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float halfDistance;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AActor* OwnerPlayer;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector StartLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector EndLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector OldStartLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float currentDistance;
private:
	FTimerHandle MoveTimer;
	FVector MoveProjectileVector;
	void Move();
	void RuleToMove(int direction);



};
