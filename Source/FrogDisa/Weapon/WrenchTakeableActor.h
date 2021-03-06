// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogDisa/InteractiveActor.h"
#include "FrogDisa/InteractiveObjectsInterface.h"
#include "WrenchTakeableActor.generated.h"

UCLASS()
class FROGDISA_API AWrenchTakeableActor : public AInteractiveActor, public IInteractiveObjectsInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWrenchTakeableActor();

	
	virtual EInteractionTypes GetInteractionType() const override{ return interactive_type; }
	void Interact() override;
	void ChangeOutlines_Implementation(bool isOutline) override { UE_LOG(LogTemp, Warning, TEXT("C++")) }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Mesh;
	const EInteractionTypes interactive_type = EInteractionTypes::EIT_Take;
	const int count_add = 1;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
