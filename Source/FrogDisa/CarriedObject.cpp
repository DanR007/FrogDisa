#include "CarriedObject.h"

// Sets default values
ACarriedObject::ACarriedObject()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));

	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);

}

// Called when the game starts or when spawned
void ACarriedObject::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetSimulatePhysics(true);
}

void ACarriedObject::DropThisObject()
{
	Mesh->SetSimulatePhysics(true);
}

void ACarriedObject::TakeThisObject()
{
	Mesh->SetSimulatePhysics(false);
}

