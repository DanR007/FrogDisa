// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItAfterShot.h"

// Sets default values
ADropItAfterShot::ADropItAfterShot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ConstructorHelpers::FClassFinder<AThrowProjectile> projectile(TEXT("Class'/Script/FrogDisa.ThrowProjectile'"));
	MeshConstruction = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshConstruction"));
	BreakableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BreakableMesh"));
	Wrench = projectile.Class;


	CableMesh1 = CreateDefaultSubobject<UCableComponent>(TEXT("Cable1"));
	CableMesh2 = CreateDefaultSubobject<UCableComponent>(TEXT("Cable2"));
	CableMesh3 = CreateDefaultSubobject<UCableComponent>(TEXT("Cable3"));
	CableMesh4 = CreateDefaultSubobject<UCableComponent>(TEXT("Cable4"));

	CableMesh1->bAttachStart = true;
	CableMesh1->bAttachEnd = true;

	CableMesh1->EndLocation = BreakableMesh->GetComponentLocation();
	CableMesh2->bAttachStart = true;
	CableMesh2->bAttachEnd = true;
	CableMesh2->EndLocation = BreakableMesh->GetComponentLocation();
	//CableMesh2->SetWorldLocation(BreakableMesh->GetComponentLocation());

	CableMesh3->bAttachStart = true;
	CableMesh3->bAttachEnd = true;
	CableMesh3->EndLocation = BreakableMesh->GetComponentLocation();
	//CableMesh3->SetWorldLocation(BreakableMesh->GetComponentLocation());

	CableMesh4->bAttachStart = true;
	CableMesh4->bAttachEnd = true;
	CableMesh4->EndLocation = BreakableMesh->GetComponentLocation();
	//CableMesh4->SetWorldLocation(BreakableMesh->GetComponentLocation());

	BreakableMesh->OnComponentBeginOverlap.AddDynamic(this, &ADropItAfterShot::OnOverlapBreakableMesh);
}

// Called when the game starts or when spawned
void ADropItAfterShot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADropItAfterShot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ADropItAfterShot::DropConstruction()
{
	CableMesh1->DestroyComponent();
	CableMesh2->DestroyComponent();
	CableMesh3->DestroyComponent();
	CableMesh4->DestroyComponent();
	BreakableMesh->DestroyComponent();
	MeshConstruction -> SetSimulatePhysics(true);
}

void ADropItAfterShot::StopDropConstruction()
{
	MeshConstruction->SetSimulatePhysics(false);
}


bool ADropItAfterShot::isABreakableMesh()
{
	TArray<AActor*> overlapArray;
	
	BreakableMesh->GetOverlappingActors(overlapArray, Wrench);
	return overlapArray.IsValidIndex(0);
}

void ADropItAfterShot::OnOverlapBreakableMesh_Implementation(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AThrowProjectile* WrenchActor = Cast<AThrowProjectile>(OtherActor);
	if (WrenchActor != nullptr && WrenchActor->GetInAirState())
	{
		DropConstruction();
	}
}