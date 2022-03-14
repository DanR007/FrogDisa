// Fill out your copyright notice in the Description page of Project Settings.


#include "CrossbowBoltActor.h"
#include "FrogDisa/Player/Movement.h"

AActor* OwnerPlayer;
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

void ACrossbowBoltActor::Interact(AActor* Player)
{
	Cast<AMovement>(Player)->shootComponent->AddAmmunition(count_add, EWeaponType::EW_CrossbowBolt);
	Destroy();
}

void ACrossbowBoltActor::Launch()
{

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	crossbow_bolt_mesh->SetSimulatePhysics(true);
	crossbow_bolt_mesh->AddImpulse(Cast<AMovement>(OwnerPlayer)->FindComponentByClass<UCameraComponent>()->
		GetForwardVector()*crossbow_bolt_mesh->GetMass() * 2000);
}

void ACrossbowBoltActor::AttachToCharacter(AActor* player_Character)
{
	OwnerPlayer = player_Character;
	this->AttachToComponent(Cast<AMovement>(OwnerPlayer)->FindComponentByClass<UCameraComponent>(), FAttachmentTransformRules::KeepWorldTransform);
	//ProjectileMesh->AttachToComponent(Cast<AMovement>(OwnerPlayer)->GetMesh(),FAttachmentTransformRules::KeepWorldTransform,TEXT("hand_RSocket"));
	SetActorRelativeLocation(FVector(0, 0, 0));
}
