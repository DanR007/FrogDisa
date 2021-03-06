// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InteractiveActor.h"
#include "InteractiveObjectsInterface.h"

#include "Components/SphereComponent.h"
#include "Components/BillboardComponent.h"

#include "CollectiblesObject.generated.h"

UCLASS()
class FROGDISA_API ACollectiblesObject : public AInteractiveActor, public IInteractiveObjectsInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectiblesObject();

	void Interact() override;

	virtual EInteractionTypes GetInteractionType() const override { return interactive_type; }
	
protected:
	const EInteractionTypes interactive_type = EInteractionTypes::EIT_Take;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* Collider;

	friend class AMovement;
};
