// Fill out your copyright notice in the Description page of Project Settings.


#include "PointOfPatrol.h"

// Sets default values
APointOfPatrol::APointOfPatrol()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
}

// Called when the game starts or when spawned
void APointOfPatrol::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APointOfPatrol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

