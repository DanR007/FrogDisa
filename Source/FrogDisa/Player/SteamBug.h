// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SteamBug.generated.h"

UCLASS()
class FROGDISA_API ASteamBug : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASteamBug();

	void SetMainCharacter(ACharacter* f_main_character);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		ACharacter* main_character;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION(BlueprintNativeEvent)
		void SetNewPosses();
private:


};
