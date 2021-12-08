// Fill out your copyright notice in the Description page of Project Settings.


#include "GrapplingObject.h"

// Sets default values
AGrapplingObject::AGrapplingObject()
{
	GrapplingPointWidget = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillBoard"));
	
	GrapplingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	GrapplingCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	RootComponent = GrapplingMesh;
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("StaticMesh'/Engine/EditorMeshes/AssetViewer/Sphere_inversenormals.Sphere_inversenormals'"));
	GrapplingMesh->SetStaticMesh(mesh.Object);
	FVector scale = FVector(0.3f, 0.3f, 0.3f);
	GrapplingMesh->SetWorldScale3D(scale);
	GrapplingCollider->SetupAttachment(RootComponent);
	GrapplingPointWidget->SetupAttachment(RootComponent);
	GrapplingCollider->SetWorldScale3D(scale * 25);
}

// Called when the game starts or when spawned
void AGrapplingObject::BeginPlay()
{
	Super::BeginPlay();
	GrapplingPointWidget->SetActive(false);
	
}

void AGrapplingObject::SetActiveObject(bool inVision)
{
	GrapplingPointWidget->SetHiddenInGame(!inVision);
}



