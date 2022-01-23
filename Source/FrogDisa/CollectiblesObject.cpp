// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectiblesObject.h"
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"

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

void ACollectiblesObject::SetActiveObject(bool inVision)
{
	
}

void ACollectiblesObject::Take(AMovement* Player)
{
	Player->AddCollectibles();
	Destroy();
}

void ACollectiblesObject::SetActiveHighlightingObject(bool ActiveHighlighting)
{
	ActiveHighlighting ? ChangeHighlightingObject(Mesh, 1.f) : ChangeHighlightingObject(Mesh, 0.f);
}

void ACollectiblesObject::ChangeHighlightingObject(UMeshComponent* change_mesh, float scale)
{
	if (change_mesh)
	{
		TArray<UMaterialInterface*> material_array = change_mesh->GetMaterials();
		for (UMaterialInterface* material_in_array : material_array)
		{
			if (material_in_array)
			{
				UMaterial* material = material_in_array->GetMaterial();
				if (material)
					material->SetScalarParameterValueEditorOnly("select", scale);
			}
		}
	}
}

