// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FrogDisa/Weapon/WeaponLogicInterface.h"
#include "FrogDisa/InteractiveObjectsInterface.h"
#include "SteamBug.generated.h"

UCLASS()
class FROGDISA_API ASteamBug : public ACharacter, public IWeaponLogicInterface, public IInteractiveObjectsInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASteamBug();

	virtual EInteractionTypes GetInteractionType() const override { return interactive_type; }
	virtual bool Launch() override;
	virtual void AttachToCharacter() override;
	virtual void Interact() override;
	virtual void ChangeOutlines_Implementation(bool isOutline) override { UE_LOG(LogTemp, Warning, TEXT("C++")) }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		ACharacter* main_character;
	UPROPERTY(EditDefaultsOnly)
		EInteractionTypes interactive_type = EInteractionTypes::EIT_Take;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION(BlueprintNativeEvent)
		void SetNewPosses();
private:
	
};
