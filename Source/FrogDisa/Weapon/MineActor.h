// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponActor.h"
#include "WeaponLogicInterface.h"
#include "MineActor.generated.h"

/**
 * 
 */
UCLASS()
class FROGDISA_API AMineActor : public AActor, public IWeaponLogicInterface
{
	GENERATED_BODY()
public:

	AMineActor();
	virtual void Launch() override;
	virtual void Create() override;
	virtual void AttachToCharacter() override;
private:

	int Time_Before_Explosion;


};
