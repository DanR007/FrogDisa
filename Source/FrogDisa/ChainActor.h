// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Components/CapsuleComponent.h"
#include "CableComponent.h"

#include "InteractiveActor.h"
#include "InteractiveObjectsInterface.h"

#include "ChainActor.generated.h"

UCLASS()
class FROGDISA_API AChainActor : public AInteractiveActor, public IInteractiveObjectsInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChainActor();

	void Interact() override;

	virtual EInteractionTypes GetInteractionType() const override { return interactive_type; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCapsuleComponent* trigger_component;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCableComponent* cable_component;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool is_interact = false;

};
