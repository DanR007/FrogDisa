// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponActor.h"
#include "GameFramework/Actor.h"
#include "MineActor.generated.h"

/**
 * 
 */
UCLASS()
class FROGDISA_API AMineActor : public AActor, public AWeaponActor
{
	GENERATED_BODY()
public:

	AMineActor();
	void Create() override;


private:

	int Time_Before_Explosion;


};
