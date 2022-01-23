// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/Interface.h"
#include "WeaponLogicInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponLogicInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FROGDISA_API IWeaponLogicInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void Launch() {}
	virtual void Create() {}
	virtual void AttachToCharacter(AActor* player_Character) {}
};
