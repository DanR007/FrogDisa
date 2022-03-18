// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Player/Movement.h"
#include "InteractiveObjectsInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractiveObjectsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FROGDISA_API IInteractiveObjectsInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
		void ChangeOutlines(bool isOutline);

	virtual void Interact() {}
};
