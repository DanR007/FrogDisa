// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolActor.h"
#include "FrogDisa/AI/NPCPawn.h"
#include "FrogDisa/DefaultVariables.h"

// Sets default values
APistolActor::APistolActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	pistol_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pistol Mesh"));
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

bool APistolActor::Launch()
{
	ANPCPawn* actor;
	FHitResult hitPoint;
	FVector start = PlayerActor->Camera->GetComponentLocation(), end = start + PlayerActor->Camera->GetForwardVector() * PistolRangeDistance;
	if (GetWorld()->LineTraceSingleByChannel(hitPoint, start, end, ECollisionChannel::ECC_Visibility))
	{
		actor = Cast<ANPCPawn>(hitPoint.GetActor());
		if (actor)
		{
			actor->Die();
		}
	}
	return false;
}

void APistolActor::AttachToCharacter()
{
	if (PlayerActor)
		this->AttachToActor(PlayerActor, FAttachmentTransformRules::KeepWorldTransform);
	//ProjectileMesh->AttachToComponent(Cast<AMovement>(OwnerPlayer)->GetMesh(),FAttachmentTransformRules::KeepWorldTransform,TEXT("hand_RSocket"));
	SetActorRelativeLocation(FVector(100, 100, 0));
}
