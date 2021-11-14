// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakingObject.h"

// Sets default values
ABreakingObject::ABreakingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	RootComponent = BoxCollider;

	MeshConstruction1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh1"));
	MeshConstruction2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh2"));
	MeshConstruction3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh3"));
	MeshConstruction4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh4"));
	MeshConstruction5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh5"));
	MeshConstruction6 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh6"));
	MeshConstruction7 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh7"));

	MeshConstruction1->SetupAttachment(RootComponent);
	MeshConstruction2->SetupAttachment(RootComponent);
	MeshConstruction3->SetupAttachment(RootComponent);
	MeshConstruction4->SetupAttachment(RootComponent);
	MeshConstruction5->SetupAttachment(RootComponent);
	MeshConstruction6->SetupAttachment(RootComponent);
	MeshConstruction7->SetupAttachment(RootComponent);

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ABreakingObject::OnOverlapMesh);

	ConstructorHelpers::FClassFinder<AThrowProjectile> projectile(TEXT("Class'/Script/FrogDisa.ThrowProjectile'"));
	WrenchClass = projectile.Class;

	MeshArray = { MeshConstruction1, MeshConstruction2, MeshConstruction3 , MeshConstruction4, MeshConstruction5, MeshConstruction6,MeshConstruction7};

	time_Before_Destroy = 3;
	//MeshConstruction1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh1"));
}

// Called when the game starts or when spawned
void ABreakingObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABreakingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (BoxCollider && WrenchOverlap &&
		Cast<AMovement>(WrenchOverlap->GetOwnerPlayer()) ->MeleeAttackIsActive)
	{
		TSet<AActor*> OverlappingActors;
		BoxCollider->GetOverlappingActors(OverlappingActors, WrenchClass);
		if(OverlappingActors.GetMaxIndex() > 0)
			ABreakingObject::BreakMesh(WrenchOverlap);
	}
}

void ABreakingObject::OnOverlapMesh_Implementation(class UPrimitiveComponent* OverlappedComp,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	WrenchOverlap = Cast<AThrowProjectile>(OtherActor);
	if (WrenchOverlap && WrenchOverlap-> GetInAirState())
	{
		ABreakingObject::BreakMesh(WrenchOverlap);
	}
}

void ABreakingObject::BreakMesh(AThrowProjectile* Wrench)
{

	for (UStaticMeshComponent* mesh : MeshArray)
	{
		mesh->SetSimulatePhysics(true);
		mesh->AddImpulse(Wrench->GetActorLocation() -
			mesh->GetComponentLocation());
		
	}

	BoxCollider->DestroyComponent();

	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &ABreakingObject::DestroyAllParts, 0.01f, false, time_Before_Destroy);

}


void ABreakingObject::DestroyAllParts()
{
	GetWorldTimerManager().ClearTimer(DestroyTimer);
	Destroy();
}


