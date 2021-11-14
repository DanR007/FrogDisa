// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableObject.h"
#include "UObject/ConstructorHelpers.h"
// Sets default values
AMovableObject::AMovableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MovableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MovableMesh"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("StaticMesh'/Engine/EngineMeshes/Cube.Cube'"));
	MovableMesh->SetStaticMesh(mesh.Object);
	MovableMesh->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void AMovableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMovableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMovableObject::SetMovable()
{
	MovableMesh->SetSimulatePhysics(true);
}

void AMovableObject::SetStatic()
{
	MovableMesh->SetSimulatePhysics(false);
}

