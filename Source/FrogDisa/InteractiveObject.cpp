// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveObject.h"
#include "InteractiveObjectComponent.h"
#include "UObject/ConstructorHelpers.h"
// Sets default values
AInteractiveObject::AInteractiveObject()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	Interactive = CreateDefaultSubobject<UInteractiveObjectComponent>(TEXT("Interactive"));
	//ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	//Mesh->SetStaticMesh(mesh.Object);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	//Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
}

// Called when the game starts or when spawned
void AInteractiveObject::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetSimulatePhysics(true);
}

void AInteractiveObject::DropThisObject()
{
	Mesh->SetSimulatePhysics(true);
}

void AInteractiveObject::TakeThisObject()
{
	Mesh->SetSimulatePhysics(false);
}

