// Fill out your copyright notice in the Description page of Project Settings.


#include "WrenchTakeableActor.h"
#include "Materials/Material.h"
#include "FrogDisa/Player/EProjectiles.h"
// Sets default values
AWrenchTakeableActor::AWrenchTakeableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
}

// Called when the game starts or when spawned
void AWrenchTakeableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWrenchTakeableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWrenchTakeableActor::Take(AMovement* Player)
{
	Player->shootComponent->AddAmmunition(count_add, EWeaponType::EW_Wrench);
	Destroy();
}

void AWrenchTakeableActor::SetActiveHighlightingObject(bool ActiveHighlighting)
{

	ActiveHighlighting ? ChangeHighlightingObject(Mesh, 1.f) : ChangeHighlightingObject(Mesh, 0.f);

}

void AWrenchTakeableActor::ChangeHighlightingObject(UMeshComponent* change_mesh, float scale)
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