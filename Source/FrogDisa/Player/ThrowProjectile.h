// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

#include "FrogDisa/Weapon/WeaponActor.h"
#include "FrogDisa/Weapon/BasicWeaponActor.h"
#include "FrogDisa/Weapon/WeaponLogicInterface.h"

#include "ThrowProjectile.generated.h"

UCLASS()
class FROGDISA_API AThrowProjectile : public AActor, public IWeaponLogicInterface
{
	GENERATED_BODY()

public:

	
	// Sets default values for this actor's properties
	AThrowProjectile();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
		void ReturnToCharacter();
	//UFUNCTION(BlueprintCallable)
	virtual void Launch() override;
	virtual void Create() override;
	//virtual void OnActorBeginOverlap() override;
	void AttachToCharacter(AActor* player_Character) override;
	bool GetInAirState();

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
