// Fill out your copyright notice in the Description page of Project Settings.


#include "CrossbowBoltActor.h"
#include "FrogDisa/DefaultVariables.h"


// Sets default values
ACrossbowBoltActor::ACrossbowBoltActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	crossbow_bolt_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Crossbow_bolt_mesh"));
	crossbow_bolt_mesh->SetRelativeRotation(FRotator(90, 180, 0));
}

// Called when the game starts or when spawned
void ACrossbowBoltActor::BeginPlay()
{
	Super::BeginPlay();
	damage = 10;
}

// Called every frame
void ACrossbowBoltActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACrossbowBoltActor::Interact()
{
	PlayerActor->shootComponent->AddAmmunition(count_add, EWeaponType::EW_CrossbowBolt);
	Destroy();
}

bool ACrossbowBoltActor::Launch()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	crossbow_bolt_mesh->SetSimulatePhysics(true);
	crossbow_bolt_mesh->AddImpulse(PlayerActor->FindComponentByClass<UCameraComponent>()->
		GetForwardVector()*crossbow_bolt_mesh->GetMass() * 2000);
	crossbow_bolt_mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);
	return true;
}

void ACrossbowBoltActor::AttachToCharacter()
{
	if (PlayerActor && PlayerActor->FindComponentByClass<UCameraComponent>())
	{
		this->AttachToComponent(PlayerActor->FindComponentByClass<UCameraComponent>(), FAttachmentTransformRules::KeepWorldTransform);
		//ProjectileMesh->AttachToComponent(Cast<AMovement>(OwnerPlayer)->GetMesh(),FAttachmentTransformRules::KeepWorldTransform,TEXT("hand_RSocket"));
		SetActorRelativeLocation(FVector(0, 0, 0));
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Spawn spawn spaaaaaaaawn");
		crossbow_bolt_mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
	}
}
