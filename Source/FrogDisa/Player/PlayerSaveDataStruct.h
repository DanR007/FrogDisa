// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerSaveDataStruct.generated.h"

USTRUCT(BlueprintType)
struct FPlayerSaveData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool isHaveABug;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool isBearObject;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int countCollectibles;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int stoneCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FTransform PlayerTransform;

};