// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EProjectiles.h"
#include "Slate/SlateBrushAsset.h"

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

	AActor* InteractiveActor;

	UStaticMeshComponent* InteractiveMesh;

	FCollisionQueryParams CollisionParams;
	
public:

	void CheckInteractiveObject();

	bool TakeInteractiveObject();

	void DropInteractiveObject(float ImpulseValue);

	void DetachInteractiveFromParent();

	AActor* GetInteractiveActor() { return InteractiveActor; }
	void SetInteractiveActorNull() { InteractiveActor = nullptr; }

	void InteractionWithObject();

	EInteractionTypes GetInteractiveType() const { return current_interaction_type; };

private:
	const float MaximumCollectibleObjectDistance = 400.f;
	const float DistanceInteractiveObject = 120.f;

	EInteractionTypes current_interaction_type = EInteractionTypes::EIT_None;

	void SetNullInteractiveObject();
};
