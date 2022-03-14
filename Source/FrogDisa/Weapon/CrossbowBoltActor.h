// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponLogicInterface.h"
#include "BasicWeaponActor.h"
#include "FrogDisa/InteractiveObjectsInterface.h"
#include "CrossbowBoltActor.generated.h"

UCLASS()
class FROGDISA_API ACrossbowBoltActor : public ABasicWeaponActor, public IWeaponLogicInterface, public IInteractiveObjectsInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrossbowBoltActor();

	void Interact(AActor* Player) override;

	void ChangeOutlines_Implementation(bool isOutline) override { UE_LOG(LogTemp, Warning, TEXT("C++")) }
	void Launch() override;
	void AttachToCharacter(AActor* player_Character) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* crossbow_bolt_mesh;

	const int count_add = 1;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};
