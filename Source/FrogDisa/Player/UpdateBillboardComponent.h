// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "FrogDisa/GrapplingObject.h"
#include "FrogDisa/CollectiblesObject.h"



#include "UpdateBillboardComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FROGDISA_API UUpdateBillboardComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUpdateBillboardComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	const float MinimumGrapplingDistance = 200.f;
	const float MaximumGrapplingDistance = 5000.f;
	const float MaximumCollectibleObjectDistance = 400.f;

public:	

	AGrapplingObject* ActorGrapplingPoint;
	ACollectiblesObject* ActorCollectibleObject;

	void CheckGrapplingPoint(bool isGrappling, FVector &HitLocation);

	void CheckCollectibleActor();
};
