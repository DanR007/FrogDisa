// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrossbowBoltActor.h"
#include "TranquilizerBoltActor.generated.h"

/**
 * 
 */
UCLASS()
class FROGDISA_API ATranquilizerBoltActor : public ACrossbowBoltActor
{
	GENERATED_BODY()
	
public:
	void AttachToCharacter() override;
};
