// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Components/TimelineComponent.h"
#include "TimerManager.h"
// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.f;
	Health = MaxHealth / 2.f;
	HealthPercentage = Health / MaxHealth;
	PreviousHealth = HealthPercentage;

	MaxStamina = 100;
	Stamina = MaxStamina / 2;
	StaminaPercentage = Stamina / MaxStamina;
	PreviousStamina = StaminaPercentage;
	IsRecharging = true;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	}
	
	if (StaminaPercentage < 1.f)
	{
		IsRecharging = true;
		Owner->GetWorldTimerManager().SetTimer(StaminaRechargeTimer, this, &UHealthComponent::SetStaminaChange, 0.1f, true, 0.f);
	}
	else
	{
		IsRecharging = false;
	}
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
}



float UHealthComponent::GetHealthPercentage()
{
	return HealthPercentage;
}

float UHealthComponent::GetStamina()
{
	return StaminaPercentage;
}


void UHealthComponent::UpdateHealth(float HealthChange)
{
	Health += HealthChange;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
	PreviousHealth = HealthPercentage;
	HealthPercentage = Health / MaxHealth;
}

void UHealthComponent::UpdateStamina(float StaminaChange)
{
	
	Stamina += StaminaChange;
	Stamina = FMath::Clamp(Stamina, 0.0f, MaxStamina);
	PreviousStamina = StaminaPercentage;
	StaminaPercentage = Stamina / MaxStamina;
	if (StaminaPercentage == 1.f)
	{
		Owner->GetWorldTimerManager().ClearTimer(StaminaRechargeTimer);
	}
	else
	{
		if (StaminaChange < 0.f)
		{
			Owner->GetWorldTimerManager().ClearTimer(StaminaRechargeTimer);
			GetWorld()->GetTimerManager().SetTimer(StaminaRechargeTimer, this, &UHealthComponent::SetStaminaChange, 0.1f, true, 1.f);
			//Owner->GetWorldTimerManager().PauseTimer(StaminaRechargeTimer);
			
		}
	}
}

void UHealthComponent::SetStaminaChange()
{
	UpdateStamina(0.4f);
	//if (StaminaChange < 0)
	//{
		//if(Owner->GetWorldTimerManager().IsTimerActive(StaminaRechargeTimer) == true)
			//Owner->GetWorldTimerManager().ClearTimer(StaminaRechargeTimer);

		//GetWorld()->GetTimerManager().SetTimer(StaminaRechargeTimer, this, &UHealthComponent::SetStaminaChange, 0.2f, true, 0.f);
	//}
	//GetOwner()->Ïåó
}

