// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Collectibles.h"
#include "Components/SphereComponent.h"
#include "Components/BillboardComponent.h"
#include "CollectiblesObject.generated.h"

UCLASS()
class FROGDISA_API ACollectiblesObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectiblesObject();
	void SetActiveObject(bool inVision);
	UCollectibles* CollectiblesComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* Collider;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBillboardComponent* CollectiblesPointWidget;
};
