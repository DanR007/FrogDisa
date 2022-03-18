// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogDisa/Weapon/WeaponLogicInterface.h"
#include "ThrowingStone.generated.h"

UCLASS()
class FROGDISA_API AThrowingStone : public AActor, public IWeaponLogicInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowingStone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* StoneMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ThrowStone(FVector VectorToTarget);
	
	virtual void Launch() override{ GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "Stone"); } ;
	virtual void Create() override { GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "Stone"); };
	virtual void AttachToCharacter(AActor* player_Character) override;
};
