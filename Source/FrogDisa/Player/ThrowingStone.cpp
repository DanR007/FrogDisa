// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowingStone.h"

// Sets default values
AThrowingStone::AThrowingStone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	StoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StoneMesh"));
	StoneMesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AThrowingStone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThrowingStone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AThrowingStone::ThrowStone(FVector VectorToTarget)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ffffffffffff"));
	StoneMesh->AddImpulse(VectorToTarget * 700);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, VectorToTarget.ToString());
}

void AThrowingStone::AttachToCharacter(AActor* player_Character)
{
	OwnerPlayer = player_Character;
	this->AttachToActor(OwnerPlayer, FAttachmentTransformRules::KeepWorldTransform);
	//ProjectileMesh->AttachToComponent(Cast<AMovement>(OwnerPlayer)->GetMesh(),FAttachmentTransformRules::KeepWorldTransform,TEXT("hand_RSocket"));
	SetActorRelativeLocation(FVector(100, 100, 0));
}