// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectiblesObject.h"
#include "UObject/ConstructorHelpers.h"
// Sets default values
ACollectiblesObject::ACollectiblesObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	CollectiblesPointWidget = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillBoard"));
	CollectiblesComponent = CreateDefaultSubobject<UCollectibles>(TEXT("CollectiblesComponent"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Collider = CreateDefaultSubobject< USphereComponent>(TEXT("Collider"));
	RootComponent = Mesh;
	CollectiblesPointWidget->SetupAttachment(RootComponent);
	CollectiblesPointWidget->SetHiddenInGame(true);
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

void ACollectiblesObject::SetActiveObject(bool inVision)
{
	CollectiblesPointWidget->SetHiddenInGame(!inVision);
}

