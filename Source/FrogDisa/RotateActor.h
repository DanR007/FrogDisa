// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/ThrowProjectile.h"
#include "RotateActor.generated.h"

UCLASS()
class FROGDISA_API ARotateActor : public AActor
{
	GENERATED_BODY()
private:
	TSubclassOf<AThrowProjectile> PlayerProjectile;
public:	
	// Sets default values for this actor's properties
	ARotateActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* TargetToStopSomething;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* RotatingMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
