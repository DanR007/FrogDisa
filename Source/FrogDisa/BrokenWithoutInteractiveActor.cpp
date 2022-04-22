// Fill out your copyright notice in the Description page of Project Settings.


#include "BrokenWithoutInteractiveActor.h"
#include "RepairInteractiveObject.h"
#include "FrogDisa/DefaultVariables.h"
// Sets default values
ABrokenWithoutInteractiveActor::ABrokenWithoutInteractiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	ColliderToCheckInteractiveObject = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	RotateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotateMesh"));
	RootComponent = MainMesh;
	ColliderToCheckInteractiveObject->SetupAttachment(RootComponent);
	RotateMesh->SetupAttachment(RootComponent);

	isBroken = true;
}

// Called when the game starts or when spawned
void ABrokenWithoutInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABrokenWithoutInteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!isBroken)
	{
		RotateMesh->AddRelativeRotation(FRotator(0, 1, 0));
	}
}

void ABrokenWithoutInteractiveActor::CheckOverlappingActor(AActor* OtherActor)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, "Begin");
	ARepairInteractiveObject* OverlapObject = Cast<ARepairInteractiveObject>(OtherActor);
	if (OverlapObject && OverlapObject->Name == Need_Name && isBroken)
	{
		if(PlayerActor->InteractiveComponent)
			PlayerActor->InteractiveComponent->DetachInteractiveFromParent();
		OverlapObject->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		OverlapObject->SetActorRelativeLocation(ColliderToCheckInteractiveObject->GetRelativeLocation());
		
		isBroken = false;
		ColliderToCheckInteractiveObject->DestroyComponent();
	}
}