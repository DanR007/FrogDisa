// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootComponent.h"
#include "Camera/CameraComponent.h"
#include "EProjectiles.h"

#include "FrogDisa/Weapon/MineActor.h"
#include "FrogDisa/Weapon/CrossbowBoltActor.h"
#include "FrogDisa/DefaultVariables.h"

#include <vector>
#include <map>


//#include "UObject/ConstructorHelpers.h"
// Sets default values for this component's properties

std::map<EWeaponType, std::pair<TSubclassOf<AActor>, int /*count ammunition*/>> weapon_map;

UShootComponent::UShootComponent()
{
	ConstructorHelpers::FClassFinder<AThrowProjectile> projectile_bp(TEXT("/Game/Blueprint/BP_MyThrowProjectile"));
	BlueprintWrench = projectile_bp.Class;
	ConstructorHelpers::FClassFinder<ACrossbowBoltActor> crossbow_letal_projectile_bp(TEXT("/Game/Blueprint/Weapons/BP_CrossbowBoltActor"));
	
	ConstructorHelpers::FClassFinder<AThrowingStone> stoneProjectiles(TEXT("/Game/Blueprint/BP_ThrowingStone"));
	StoneClass = stoneProjectiles.Class;

	ConstructorHelpers::FClassFinder<AMineActor> mineActorClass(TEXT("Class'/Script/FrogDisa.MineActor'"));

	weapon_map[EWeaponType::EW_Wrench].first = BlueprintWrench;
	weapon_map[EWeaponType::EW_Wrench].second = 0;

	weapon_map[EWeaponType::EW_Stone].first = StoneClass;
	weapon_map[EWeaponType::EW_Stone].second = 0;

	weapon_map[EWeaponType::EW_Mine].first = mineActorClass.Class;
	weapon_map[EWeaponType::EW_Mine].second = 0;

	weapon_map[EWeaponType::EW_CrossbowBolt].first = crossbow_letal_projectile_bp.Class;
	weapon_map[EWeaponType::EW_CrossbowBolt].second = 0;
}


// Called when the game starts
void UShootComponent::BeginPlay()
{
	Super::BeginPlay();
	PlayerActor = Cast<AMovement>(GetOwner());
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	UE_LOG(LogTemp, Warning, TEXT("SPAWN"))
}

AActor *UShootComponent::GetActorWrench()
{
	return Cast<AActor>(Wrench);
}

void UShootComponent::Fire()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Shoot");
	//Wrench = Cast<AThrowProjectile>(Current_Weapon);
	if (Current_Weapon && PlayerActor->grapplingComponent->GetGrapplingModeActive() == false)
	{
		IWeaponLogicInterface* weaponLogicInterface = Cast<IWeaponLogicInterface>(Current_Weapon);

		if (weaponLogicInterface)
		{
			weaponLogicInterface->Launch();
			weapon_map[PlayerActor->GetCurrentWeaponType()].second--;
		}
	}
	//Wrench->Launch();
}

void UShootComponent::SwitchProjectile()
{
	EWeaponType currentType = PlayerActor->GetCurrentWeaponType();
	if (weapon_map[currentType].second > 0)//if we have ammo spawn projectile else palm will be empty
	{
		if (Current_Weapon)
		{
			Current_Weapon->Destroy();
		}
		Current_Weapon = GetWorld()->SpawnActor<AActor>(weapon_map[currentType].first, GetOwner()->FindComponentByClass<UCameraComponent>()->GetComponentTransform());
		Cast<IWeaponLogicInterface>(Current_Weapon)->AttachToCharacter(PlayerActor);
		
	}
	else
	{
		if(Current_Weapon)
			Current_Weapon->Destroy();
		Current_Weapon = nullptr;
		
	}
}

void UShootComponent::AddAmmunition(int ammunition_count, EWeaponType ammunition_type)
{
	weapon_map[ammunition_type].second += ammunition_count;
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::FromInt(weapon_map[ammunition_type].second));
}

