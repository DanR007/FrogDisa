// Fill out your copyright notice in the Description page of Project Settings.


#include "MineActor.h"
#include "FrogDisa/AI/NPCPawn.h"
#include "FrogDisa/DefaultVariables.h"

AMineActor::AMineActor()
{
	mainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main mesh"));
	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	RootComponent = mainMesh;
	ExplosionCollision->SetupAttachment(mainMesh);
	queryParams.AddIgnoredActor(this);
	ExplosionCollision->SetSphereRadius(0);
}


bool AMineActor::Launch()
{
	FVector start = PlayerActor->Camera->GetComponentLocation(), end = PlayerActor->Camera->GetForwardVector() * 200 + start;
	FHitResult hitPoint;
	if (GetWorld()->LineTraceSingleByChannel(hitPoint, start, end, ECollisionChannel::ECC_Visibility, queryParams))
	{
		this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		SetActorLocation(hitPoint.Location + FVector(0, 0, 10));
		
		if (hitPoint.GetActor())
			SetActorRotation(hitPoint.GetActor()->GetActorForwardVector().Rotation());
		else
			SetActorRotation(FRotator::ZeroRotator);
		ExplosionCollision->SetSphereRadius(200.f);

		return true;
	}
	else
	{
		PlayerActor->weaponComponent->AddAmmunition(1, EWeaponType::EW_Mine);
		return false;
	}
}


void AMineActor::AttachToCharacter()
{
	if(PlayerActor)
	this->AttachToActor(PlayerActor, FAttachmentTransformRules::KeepWorldTransform);
	//ProjectileMesh->AttachToComponent(Cast<AMovement>(OwnerPlayer)->GetMesh(),FAttachmentTransformRules::KeepWorldTransform,TEXT("hand_RSocket"));
	SetActorRelativeLocation(FVector(100, 100, 0));
}

void AMineActor::Interact()
{
	PlayerActor->weaponComponent->AddAmmunition(1, EWeaponType::EW_Mine);
	Destroy();
}

void AMineActor::Explosion()
{
	TArray<AActor*> array_enemy;
	FHitResult hitPoint;
	
	ExplosionCollision->GetOverlappingActors(array_enemy, APawn::StaticClass());
	
	for (AActor* actor : array_enemy)
	{
		ANPCPawn* npc = Cast<ANPCPawn>(actor);
		if(npc)
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

void AMineActor::CallExplosionParticle_Implementation()
{

}