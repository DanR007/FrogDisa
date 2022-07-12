// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "EProjectiles.h"

#include "FrogDisa/Weapon/MineActor.h"
#include "FrogDisa/Weapon/GrenadeActor.h"
#include "FrogDisa/Weapon/CrossbowBoltActor.h"
#include "FrogDisa/Weapon/PistolActor.h"
#include "FrogDisa/Weapon/TranquilizerBoltActor.h"

#include "FrogDisa/DefaultVariables.h"


#include <vector>
#include <map>


//#include "UObject/ConstructorHelpers.h"
// Sets default values for this component's properties

std::map<EWeaponType, std::pair<TSubclassOf<AActor>, int /*count ammunition*/>> weapon_map;

UWeaponComponent::UWeaponComponent()
{
	ConstructorHelpers::FClassFinder<AThrowProjectile> projectile_bp(TEXT("/Game/Blueprint/BP_MyThrowProjectile"));
	BlueprintWrench = projectile_bp.Class;
	ConstructorHelpers::FClassFinder<ACrossbowBoltActor> crossbow_letal_projectile_bp(TEXT("/Game/Blueprint/Weapons/BP_CrossbowBoltActor"));
	
	ConstructorHelpers::FClassFinder<AThrowingStone> stoneProjectiles(TEXT("/Game/Blueprint/BP_ThrowingStone"));
	StoneClass = stoneProjectiles.Class;

	ConstructorHelpers::FClassFinder<AMineActor> mineActorClass(TEXT("/Game/Blueprint/Weapons/BP_MineActor"));
	ConstructorHelpers::FClassFinder<AGrenadeActor> grenadeActorClass(TEXT("/Game/Blueprint/Weapons/BP_GrenadeActor"));
	ConstructorHelpers::FClassFinder<APistolActor> pistolActorClass(TEXT("/Game/Blueprint/Weapons/BP_PistolActor"));
	ConstructorHelpers::FClassFinder<ATranquilizerBoltActor> tranquilizerActorClass(TEXT("/Game/Blueprint/Weapons/BP_TranquilizerBoltActor"));
	ConstructorHelpers::FClassFinder<ASteamBug> steamBugClass(TEXT("/Game/Blueprint/BP_SteamBug"));

	weapon_map[EWeaponType::EW_None].first = StoneClass;
	weapon_map[EWeaponType::EW_None].second = 0;

	weapon_map[EWeaponType::EW_Stone].first = StoneClass;
	weapon_map[EWeaponType::EW_Stone].second = 0;

	weapon_map[EWeaponType::EW_Mine].first = mineActorClass.Class;
	weapon_map[EWeaponType::EW_Mine].second = 10;

	weapon_map[EWeaponType::EW_CrossbowBolt].first = crossbow_letal_projectile_bp.Class;
	weapon_map[EWeaponType::EW_CrossbowBolt].second = 100;

	weapon_map[EWeaponType::EW_Grenade].first = grenadeActorClass.Class;
	weapon_map[EWeaponType::EW_Grenade].second = 10;

	weapon_map[EWeaponType::EW_Pistol].first = pistolActorClass.Class;
	weapon_map[EWeaponType::EW_Pistol].second = 10;

	weapon_map[EWeaponType::EW_Tranquilizer].first = tranquilizerActorClass.Class;
	weapon_map[EWeaponType::EW_Tranquilizer].second = 10;

	weapon_map[EWeaponType::EW_Bug].first = steamBugClass.Class;
	weapon_map[EWeaponType::EW_Bug].second = 10;
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//UE_LOG(LogTemp, Warning, TEXT("SPAWN"))
}

void UWeaponComponent::Fire()
{
	if (Current_Weapon && PlayerActor->grapplingComponent->GetGrapplingModeActive() == false)
	{
		if (time_throw_attack < time_needed_to_throw)
		{
			IWeaponLogicInterface* weaponLogicInterface = Cast<IWeaponLogicInterface>(Current_Weapon);

			if (weaponLogicInterface)
			{
				if (weaponLogicInterface->Launch())
					if (weapon_map[current_weapon_type].second > 0)
					{
						Current_Weapon = GetWorld()->SpawnActor<AActor>(weapon_map[current_weapon_type].first, GetOwner()->FindComponentByClass<UCameraComponent>()->GetComponentTransform());
						Cast<IWeaponLogicInterface>(Current_Weapon)->AttachToCharacter();
					}
				weapon_map[current_weapon_type].second--;
			}
		}
		else
		{
			if (Wrench)
			{
				weapon_map[EWeaponType::EW_None].second--;
				Wrench->Launch();
			}
		}
	}
}

void UWeaponComponent::SwitchProjectile(EWeaponType type)
{
	current_weapon_type = type;
	if (weapon_map[current_weapon_type].second > 0)//if we have ammo spawn projectile else palm will be empty
	{
		if (Current_Weapon)
		{
			Current_Weapon->Destroy();
		}
		Current_Weapon = GetWorld()->SpawnActor<AActor>(weapon_map[current_weapon_type].first, GetOwner()->FindComponentByClass<UCameraComponent>()->GetComponentTransform());
		Cast<IWeaponLogicInterface>(Current_Weapon)->AttachToCharacter();
		
	}
	else
	{
		if(Current_Weapon)
			Current_Weapon->Destroy();
		Current_Weapon = nullptr;
	}
}

void UWeaponComponent::ShowWrench()
{
	Wrench = Cast<IWeaponLogicInterface>(GetWorld()->SpawnActor<AActor>(BlueprintWrench, GetOwner()->FindComponentByClass<UCameraComponent>()->GetComponentTransform()));
	Wrench->AttachToCharacter();
}

void UWeaponComponent::AddAmmunition(int ammunition_count, EWeaponType ammunition_type)
{
	weapon_map[ammunition_type].second += ammunition_count;
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::FromInt(weapon_map[ammunition_type].second));
}


int UWeaponComponent::GetCountAmmunition()
{
	return weapon_map[current_weapon_type].second;
}

void UWeaponComponent::Attack(float value)
{
	if (time_throw_attack >= time_needed_to_throw && PlayerActor->isBearObject == false)
		if (value == 0)
		{
			Fire();
			time_throw_attack = 0;
		}
		else
		{
			//Start show throw target
			time_throw_attack += value;
		}
	else
	{
		if (value > 0.f)
			if (PlayerActor->isBearObject)
			{
				PlayerActor->isBearObject = false;
				PlayerActor->InteractiveComponent->DropInteractiveObject(PlayerActor->ThrowImpulseValue);
			}
			else
			{
				if (weapon_map[EWeaponType::EW_None].second > 0)
				{
					//make attack anim
				}
			}
	}
}


EWeaponType UWeaponComponent::GetCurrentWeaponType() const
{
	return current_weapon_type;
}