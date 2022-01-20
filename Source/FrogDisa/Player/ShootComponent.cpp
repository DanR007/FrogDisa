// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootComponent.h"
#include "Camera/CameraComponent.h"
#include "EProjectiles.h"
#include "Movement.h"

#include <vector>
#include <map>

AMovement* Player_Actor;

//#include "UObject/ConstructorHelpers.h"
// Sets default values for this component's properties

UShootComponent::UShootComponent()
{
	ConstructorHelpers::FClassFinder<AThrowProjectile> projectile_bp(TEXT("/Game/Blueprint/BP_MyThrowProjectile"));
	BlueprintWrench = projectile_bp.Class;
	
	ConstructorHelpers::FClassFinder<AThrowingStone> stoneProjectiles(TEXT("/Game/Blueprint/BP_ThrowingStone"));
	StoneClass = stoneProjectiles.Class;

	//weapon_map[EP_Wrench] = std::make_pair(BlueprintWrench.Get(), BlueprintWrench);
}


// Called when the game starts
void UShootComponent::BeginPlay()
{
	Super::BeginPlay();
	Player_Actor = Cast<AMovement>(GetOwner());
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//Current_Weapon = GetWorld()->SpawnActor<AActor>(weapon_map[EP_Wrench].first, Player_Actor->FindComponentByClass<UCameraComponent>()->GetComponentTransform(), SpawnParameters);
	//Wrench = GetWorld()->SpawnActor<AThrowProjectile>(BlueprintWrench, GetOwner()->FindComponentByClass<UCameraComponent>()->GetComponentTransform(), SpawnParameters);
	//Wrench->AttachToPlayerCharacter(GetOwner());

}


AActor *UShootComponent::GetActorWrench()
{
	return Cast<AActor>(Wrench);
}

void UShootComponent::Fire_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Shoot");
	//вот здесь я хочу вызвать метод Launch() у 
	//всех типов вооружения (потому что они все унаследовали от абстрактного класса
	//AWeaponActor этот метод) но как закастить ко всем классам я не понимаю
	//я пробовал через словарь (map), но так не получалось
	//как делать появление я наверное разобрался, потому что UClass* я получаю
	//однако хардкодить через switch case (чтобы по типу оружия потом написать, к примеру, case EP_Wrench: Cast<AThrowProjectile>(CurrentWeapon); я не хочу
	//и хотел бы понять как можно сделать 
	
	//Current_Weapon->Launch();
	
	//Wrench->Launch();
}


