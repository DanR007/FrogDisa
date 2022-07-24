// Fill out your copyright notice in the Description page of Project Settings.


#include "ChainActor.h"

#include "DefaultVariables.h"

// Sets default values
AChainActor::AChainActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	cable_component = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	trigger_component = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger"));

	RootComponent = trigger_component;
	cable_component->SetupAttachment(RootComponent);
}


// Called when the game starts or when spawned
void AChainActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChainActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChainActor::Interact()
{
	if (PlayerActor)
	{
		is_interact = !is_interact;
		PlayerActor->SetIsClimbing(is_interact);
	}

}