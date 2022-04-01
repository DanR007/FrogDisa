// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolActor.h"
#include "FrogDisa/AI/NPCPawn.h"

// Sets default values
APistolActor::APistolActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APistolActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APistolActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APistolActor::Shoot()
{

}
