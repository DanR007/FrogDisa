// Fill out your copyright notice in the Description page of Project Settings.


#include "WrenchTakeableActor.h"
#include "Materials/Material.h"
#include "FrogDisa/DefaultVariables.h"
#include "FrogDisa/Player/EProjectiles.h"
// Sets default values
AWrenchTakeableActor::AWrenchTakeableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
}

// Called when the game starts or when spawned
void AWrenchTakeableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWrenchTakeableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWrenchTakeableActor::Interact()
{
	PlayerActor->weaponComponent->ShowWrench();
	Destroy();
}

