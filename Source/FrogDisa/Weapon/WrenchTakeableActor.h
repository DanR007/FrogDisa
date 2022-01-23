// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FrogDisa/ObjectTakenInterface.h"
#include "WrenchTakeableActor.generated.h"

UCLASS()
class FROGDISA_API AWrenchTakeableActor : public AActor, public IObjectTakenInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWrenchTakeableActor();

	void Take(AMovement* Player) override;
	void SetActiveHighlightingObject(bool ActiveHighlighting) override;
	void ChangeHighlightingObject(UMeshComponent* change_mesh, float scale) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Mesh;

	const int count_add = 1;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
