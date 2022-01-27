// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorLockActor.h"

// Sets default values
ADoorLockActor::ADoorLockActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Pivot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pivot"));
	MainDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = Pivot;
	MainDoorMesh->SetupAttachment(Pivot);
}

// Called when the game starts or when spawned
void ADoorLockActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoorLockActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorLockActor::Interact(AMovement* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("Interact"))
	if (isLock)
	{
		HackDoor();
	}
	else
	{
		OpenOrClose();
	}
}

void ADoorLockActor::LockIsHack()
{
	isLock = false;
}

void ADoorLockActor::HackDoor_Implementation()
{

}

void ADoorLockActor::OpenOrClose_Implementation()
{

}