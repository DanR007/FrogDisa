// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectiblesObject.h"
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"
#include "FrogDisa/DefaultVariables.h"
//#define TEST



// Sets default values
ACollectiblesObject::ACollectiblesObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Collider = CreateDefaultSubobject< USphereComponent>(TEXT("Collider"));
	RootComponent = Mesh;

	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("StaticMesh'/Engine/EditorMeshes/AssetViewer/Sphere_inversenormals.Sphere_inversenormals'"));
	Mesh->SetStaticMesh(mesh.Object);
	FVector scale = FVector(0.3f, 0.3f, 0.3f);
	Mesh->SetWorldScale3D(scale);
	Collider->SetupAttachment(RootComponent);
	Collider->SetWorldScale3D(scale * 17);
}

// Called when the game starts or when spawned
void ACollectiblesObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACollectiblesObject::Interact()
{
	PlayerActor->AddCollectibles();
	Destroy();
}


