// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponActor.h"
#include "MineActor.generated.h"

/**
 * 
 */
UCLASS()
class FROGDISA_API AMineActor : public AWeaponActor
{
	GENERATED_BODY()
public:

	virtual void Create() override;


private:

	int Time_Before_Explosion;


};
