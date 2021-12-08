// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveObject.h"

// Sets default values
AInteractiveObject::AInteractiveObject()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));

	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);

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

