// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InteractiveClass.h"

#include "Components/SphereComponent.h"
#include "Components/BillboardComponent.h"

#include "CollectiblesObject.generated.h"

UCLASS()
class FROGDISA_API ACollectiblesObject : public AActor, public IInteractiveObjectsInterface, public InteractiveClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectiblesObject();

	void Interact() override;

	
	
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
