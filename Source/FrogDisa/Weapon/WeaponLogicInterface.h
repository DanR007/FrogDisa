// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/Interface.h"
#include "WeaponLogicInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
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

	virtual bool Launch() { return false; }
	virtual void Create() {}
	virtual void AttachToCharacter() {}
};
