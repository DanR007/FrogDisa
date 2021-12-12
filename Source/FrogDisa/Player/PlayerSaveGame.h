// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSaveDataStruct.h"
#include "PlayerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class FROGDISA_API UPlayerSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FPlayerSaveData PlayerData;
};
