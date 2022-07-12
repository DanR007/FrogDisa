#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThrowProjectile.h"
#include "UObject/ConstructorHelpers.h"
#include "EProjectiles.h"
#include "ThrowingStone.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FROGDISA_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWeaponComponent();

	void SwitchProjectile(EWeaponType type);
	void Fire();

	AActor* Current_Weapon;

	TSubclassOf<AThrowProjectile> BlueprintWrench;

	TSubclassOf<AThrowingStone> StoneClass;

	void AddAmmunition(int ammunition_count, EWeaponType ammunition_type);

	UFUNCTION(BlueprintCallable)
		int GetCountAmmunition();

	UFUNCTION(BlueprintCallable)
		void Attack(float value);

	UFUNCTION(BlueprintCallable)
		EWeaponType GetCurrentWeaponType() const;

	UFUNCTION(BlueprintCallable)
		void ShowWrench();

	void ChoicePistol()
	{
		if (GetCurrentWeaponType() != EWeaponType::EW_Pistol)
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, "Pistol");
			SwitchProjectile(EWeaponType::EW_Pistol);
	}
	void ChoiceMine()
	{
		if (GetCurrentWeaponType() != EWeaponType::EW_Mine)
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, "Mine");
			SwitchProjectile(EWeaponType::EW_Mine);
	}
	void ChoiceCrossbowBolt()
	{
		if (GetCurrentWeaponType() != EWeaponType::EW_CrossbowBolt)
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, "CrossbowBolt");
			SwitchProjectile(EWeaponType::EW_CrossbowBolt);
	}
	void ChoiceTranquilizer()
	{
		if (GetCurrentWeaponType() != EWeaponType::EW_Tranquilizer)
			SwitchProjectile(EWeaponType::EW_Tranquilizer);

	}
	void ChoiceGrenade()
	{
		if (GetCurrentWeaponType() != EWeaponType::EW_Grenade)
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, "granataaaaaaaaaaaaa");
			SwitchProjectile(EWeaponType::EW_Grenade);
	}
	void ChoiceBug()
	{
		if (GetCurrentWeaponType() != EWeaponType::EW_Bug)
			SwitchProjectile(EWeaponType::EW_Bug);
	}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
private:

	float time_throw_attack = 0.f;
	const float time_needed_to_throw = 15.f;;

	EWeaponType current_weapon_type = EWeaponType::EW_None;

	AThrowingStone* StoneActor;

	IWeaponLogicInterface* Wrench;

};
