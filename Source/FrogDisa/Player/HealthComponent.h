// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FROGDISA_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
		void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float HealthPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float PreviousHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		bool Invulnerability;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
		float MaxStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
		float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float StaminaPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float PreviousStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		UCurveFloat* StaminaCurve;

	bool IsRecharging;
	float StaminaValue;
	AActor* Owner;
	FTimerHandle StaminaRechargeTimer;
	/** Get Health */
	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetHealthPercentage();

	/** Get Stamina */
	UFUNCTION(BlueprintCallable, Category = "Stamina")
		float GetStamina();

	/** Update Health */
	UFUNCTION(BlueprintCallable, Category = "Health")
		void UpdateHealth(float HealthChange);

	/** Update Stamina */
	UFUNCTION(BlueprintCallable, Category = "Stamina")
		void UpdateStamina(float StaminaChange);

	UFUNCTION()
		void SetStaminaChange();
};
