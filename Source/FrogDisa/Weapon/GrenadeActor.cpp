// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeActor.h"
#include "FrogDisa/AI/NPCPawn.h"
#include "FrogDisa/DefaultVariables.h"

// Sets default values
AGrenadeActor::AGrenadeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	mainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main mesh"));
	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));

	RootComponent = mainMesh;
	ExplosionCollision->SetupAttachment(mainMesh);
	ExplosionCollision->SetRelativeLocation(FVector::ZeroVector);
}

// Called when the game starts or when spawned
void AGrenadeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrenadeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGrenadeActor::Launch()
{
	this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	mainMesh->SetSimulatePhysics(true);
	mainMesh->AddImpulse(PlayerActor->FindComponentByClass<UCameraComponent>()->
		GetForwardVector() * mainMesh->GetMass() * 500);
	mainMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimer, this, &AGrenadeActor::Explosion, 4.f, true, 4.f);

	return true;
}


void AGrenadeActor::AttachToCharacter()
{
	if (PlayerActor)
		this->AttachToActor(PlayerActor, FAttachmentTransformRules::KeepWorldTransform);
	//ProjectileMesh->AttachToComponent(Cast<AMovement>(OwnerPlayer)->GetMesh(),FAttachmentTransformRules::KeepWorldTransform,TEXT("hand_RSocket"));
	SetActorRelativeLocation(FVector(100, 100, 0));
}

void AGrenadeActor::Interact()
{
	PlayerActor->shootComponent->AddAmmunition(1, EWeaponType::EW_Mine);
}

void AGrenadeActor::Explosion()
{
	////GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "Damage");
	TArray<AActor*> array_enemy;
	FHitResult hitPoint;
	FCollisionQueryParams queryParams;
	ExplosionCollision->GetOverlappingActors(array_enemy, APawn::StaticClass());
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::FromInt(array_enemy.Num()));
	queryParams.AddIgnoredActor(this);
	for (AActor* actor : array_enemy)
	{
		ANPCPawn* npc = Cast<ANPCPawn>(actor);
		if (npc)
			if (GetWorld()->LineTraceSingleByChannel(hitPoint, GetActorLocation(), actor->GetActorLocation(), ECollisionChannel::ECC_Visibility, queryParams))
			{
				if (npc == hitPoint.GetActor())
				{
					//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "Damage");
					npc->Die();
				}
			}
	}
	
	CallExplosionParticle();

	Destroy();
}

void AGrenadeActor::CallExplosionParticle_Implementation()
{

}