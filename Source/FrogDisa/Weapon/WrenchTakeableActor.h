// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FrogDisa/InteractiveObjectsInterface.h"
#include "WrenchTakeableActor.generated.h"

UCLASS()
class FROGDISA_API AWrenchTakeableActor : public AActor, public IInteractiveObjectsInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWrenchTakeableActor();

	void Interact(AActor* Player) override;

	void ChangeScalarParameter_Implementation(float scale) override { UE_LOG(LogTemp, Warning, TEXT("C++")) }
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
