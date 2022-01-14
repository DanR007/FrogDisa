// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootComponent.h"
#include "Camera/CameraComponent.h"
#include "EProjectiles.h"
#include <vector>

//#include "UObject/ConstructorHelpers.h"
// Sets default values for this component's properties
UShootComponent::UShootComponent()
{
	ConstructorHelpers::FClassFinder<AThrowProjectile> projectile_bp(TEXT("/Game/Blueprint/BP_MyThrowProjectile"));
	BlueprintWrench = projectile_bp.Class;
	
	ConstructorHelpers::FClassFinder<AThrowingStone> stoneProjectiles(TEXT("/Game/Blueprint/BP_ThrowingStone"));
	StoneClass = stoneProjectiles.Class;
}


// Called when the game starts
void UShootComponent::BeginPlay()
{
	Super::BeginPlay();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Wrench = GetWorld()->SpawnActor<AThrowProjectile>(BlueprintWrench, GetOwner()->FindComponentByClass<UCameraComponent>()->GetComponentTransform(), SpawnParameters);
	Wrench->AttachToPlayerCharacter(GetOwner());

}

void UShootComponent::ThrowProjectile(EProjectiles projectile_type)
{
	if (projectile_type == EP_Wrench)
		Wrench->Launch();
	else
	{
		FHitResult hitPoint;
		FCollisionQueryParams CollisionParams;
		FVector Start = GetOwner()->FindComponentByClass<UCameraComponent>()->GetComponentLocation(), End = GetOwner()->FindComponentByClass<UCameraComponent>()->GetForwardVector() * 5000 + Start;
		if (GetWorld()->LineTraceSingleByChannel(hitPoint, Start, End, ECC_Visibility, CollisionParams) == true)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::SanitizeFloat(0));
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			StoneActor = GetWorld()->SpawnActor<AThrowingStone>(StoneClass, GetOwner()->FindComponentByClass<UCameraComponent>()->GetComponentTransform(), SpawnParameters);
			StoneActor->ThrowStone(hitPoint.Location - Start);
		}
		
	}

}

void UShootComponent::ReturnProjectile()
{
	Wrench->ReturnToCharacter();
}

AActor *UShootComponent::GetActorWrench()
{
	return Cast<AActor>(Wrench);
}

void UShootComponent::Fire()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Shoot");
	Wrench->Launch();
}


