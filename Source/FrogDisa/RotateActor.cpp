// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateActor.h"

// Sets default values
ARotateActor::ARotateActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<AThrowProjectile> projectile(TEXT("Class'/Script/FrogDisa.ThrowProjectile'"));
	PlayerProjectile = projectile.Class;

	TargetToStopSomething = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Target"));
	RotatingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rotator"));
}

// Called when the game starts or when spawned
void ARotateActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARotateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

