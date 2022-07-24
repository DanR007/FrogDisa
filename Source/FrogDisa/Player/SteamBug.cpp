// Fill out your copyright notice in the Description page of Project Settings.


#include "SteamBug.h"
#include "FrogDisa/DefaultVariables.h"

// Sets default values
ASteamBug::ASteamBug()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASteamBug::BeginPlay()
{
	Super::BeginPlay();
	main_character = PlayerActor;
}

// Called every frame
void ASteamBug::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FHitResult hitRes;
	/*FVector start = GetActorLocation(), end = start + GetActorForwardVector() * (GetCapsuleComponent()->GetScaledCapsuleRadius() + 5);
	if (GetWorld()->LineTraceSingleByChannel(hitRes, start, end, ECollisionChannel::ECC_Visibility))
	{
		SetActorRotation(hitRes.ImpactNormal.Rotation());
	}*/
}

// Called to bind functionality to input
void ASteamBug::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASteamBug::SetNewPosses_Implementation()
{

}

bool ASteamBug::Launch()
{
	FVector start = PlayerActor->Camera->GetComponentLocation(), end = PlayerActor->Camera->GetForwardVector() * 200 + start;
	FHitResult hitPoint;
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(PlayerActor);
	if (GetWorld()->LineTraceSingleByChannel(hitPoint, start, end, ECollisionChannel::ECC_Visibility, queryParams) &&
		hitPoint.ImpactNormal == PlayerActor->GetActorUpVector())
	{
		this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		SetActorLocation(hitPoint.Location + FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleRadius() + 10));

		if (hitPoint.GetActor())
			SetActorRotation(hitPoint.GetActor()->GetActorForwardVector().Rotation());
		else
			SetActorRotation(FRotator::ZeroRotator);

		PlayerActor->SetNewPosses(this);

		return true;
	}


		return false;
	
}

void ASteamBug::AttachToCharacter()
{
	if (PlayerActor)
		this->AttachToActor(PlayerActor, FAttachmentTransformRules::KeepWorldTransform);
	//ProjectileMesh->AttachToComponent(Cast<AMovement>(OwnerPlayer)->GetMesh(),FAttachmentTransformRules::KeepWorldTransform,TEXT("hand_RSocket"));
	SetActorRelativeLocation(FVector(100, 100, 0));
}

void ASteamBug::Interact()
{
	PlayerActor->weaponComponent->AddAmmunition(1, EWeaponType::EW_Mine);
	Destroy();
}
