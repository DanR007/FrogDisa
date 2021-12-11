// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "FrogDisa/InteractiveObject.h"
#include "InteractiveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FROGDISA_API UInteractiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractiveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	AActor* Owner;
	AActor* InteractiveActor;

	UStaticMeshComponent* InteractiveMesh;

	FCollisionQueryParams CollisionParams;

	
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool TakeInteractiveObject(UStaticMeshComponent* Player_InteractiveMesh);
	bool IsZeroOverlappingActors();
	bool OverlapOnlyInteractivePuzzle();

	void DropInteractiveObject(UStaticMeshComponent* Player_InteractiveMesh);
	void ThrowInteractiveObject(UStaticMeshComponent* Player_InteractiveMesh);

	void DetachInteractiveFromParent(UStaticMeshComponent* Player_InteractiveMesh);
};
