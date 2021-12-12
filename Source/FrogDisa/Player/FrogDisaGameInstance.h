// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PlayerSaveGame.h"
#include "FrogDisaGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FROGDISA_API UFrogDisaGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	void virtual Init() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString saveSlotName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UPlayerSaveGame* PlayerSaveData;
};
