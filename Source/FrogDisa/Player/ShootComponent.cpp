// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootComponent.h"
#include "Camera/CameraComponent.h"
#include "EProjectiles.h"
#include "Movement.h"

#include "FrogDisa/Weapon/MineActor.h"

#include <vector>
#include <map>

AMovement* Player_Actor;

//#include "UObject/ConstructorHelpers.h"
// Sets default values for this component's properties

std::map<EWeaponType, TSubclassOf<AActor>> weapon_map;

UShootComponent::UShootComponent()
{
	ConstructorHelpers::FClassFinder<AThrowProjectile> projectile_bp(TEXT("/Game/Blueprint/BP_MyThrowProjectile"));
	BlueprintWrench = projectile_bp.Class;
	
	ConstructorHelpers::FClassFinder<AThrowingStone> stoneProjectiles(TEXT("/Game/Blueprint/BP_ThrowingStone"));
	StoneClass = stoneProjectiles.Class;

	ConstructorHelpers::FClassFinder<AMineActor> mineActorClass(TEXT("Class'/Script/FrogDisa.MineActor'"));

	weapon_map[EWeaponType::EW_Wrench] = BlueprintWrench;
	weapon_map[EWeaponType::EW_Stone] = StoneClass;
	weapon_map[EWeaponType::EW_Mine] = mineActorClass.Class;
}


// Called when the game starts
void UShootComponent::BeginPlay()
{
	Super::BeginPlay();
	Player_Actor = Cast<AMovement>(GetOwner());
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	UE_LOG(LogTemp, Warning, TEXT("SPAWN"))
	//Current_Weapon = GetWorld()->SpawnActor<AActor>(BlueprintWrench, Player_Actor->FindComponentByClass<UCameraComponent>()->GetComponentTransform(), SpawnParameters);
	//Wrench = GetWorld()->SpawnActor<AThrowProjectile>(BlueprintWrench, GetOwner()->FindComponentByClass<UCameraComponent>()->GetComponentTransform(), SpawnParameters);
	//Wrench->AttachToPlayerCharacter(GetOwner());
	//Current_Weapon = Wrench;
}


AActor *UShootComponent::GetActorWrench()
{
	return Cast<AActor>(Wrench);
}

void UShootComponent::Fire()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Shoot");
	//вот здесь я хочу вызвать метод Launch() у 
	//всех типов вооружения (потому что они все унаследовали от абстрактного класса
	//AWeaponActor этот метод) но как закастить ко всем классам я не понимаю
	//я пробовал через словарь (map), но так не получалось
	//как делать появление я наверное разобрался, потому что UClass* я получаю
	//однако хардкодить через switch case (чтобы по типу оружия потом написать, к примеру, case EW_Wrench: Cast<AThrowProjectile>(CurrentWeapon); я не хочу
	//и хотел бы понять как можно сделать
	//Wrench = Cast<AThrowProjectile>(Current_Weapon);
	if (Current_Weapon)
	{
		IWeaponLogicInterface* weaponLogicInterface = Cast<IWeaponLogicInterface>(Current_Weapon);

		if (weaponLogicInterface)
		{
			weaponLogicInterface->Launch();
		}
	}
	//Wrench->Launch();
}

void UShootComponent::SwitchProjectile()
{
	EWeaponType currentType = Player_Actor->GetCurrentWeaponType();
	if (Current_Weapon)
	{
		Current_Weapon->Destroy();
	}
	Current_Weapon = GetWorld()->SpawnActor<AActor>(weapon_map[currentType], GetOwner()->FindComponentByClass<UCameraComponent>()->GetComponentTransform());
}

