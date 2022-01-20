// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//UENUM(BlueprintType)
UENUM(BlueprintType)
enum class EWeaponType: uint8
{
	EW_Stone UMETA(DisplayName="EW_Stone"),
	EW_Tranquilizer UMETA(DisplayName = "EW_Tranquilizer"),
	EW_ThrowingKnife UMETA(DisplayName = "EW_ThrowingKnife"),
	EW_CrossbowBolt UMETA(DisplayName = "EW_CrossbowBolt"),
	EW_Flashbang UMETA(DisplayName = "EW_Flashbang"),
	EW_Grenade UMETA(DisplayName = "EW_Grenade"),
	EW_SmokeGrenade UMETA(DisplayName = "EW_SmokeGrenade"),
	EW_SmokeMine UMETA(DisplayName = "EW_SmokeMine"),
	EW_FlashbangMine UMETA(DisplayName = "EW_FlashbangMine"),
	EW_Mine UMETA(DisplayName = "EW_Mine"),
	EW_Wrench UMETA(DisplayName = "EW_Wrench")
};
//UENUM()
enum ERotatorDirection
{
	ERD_Forward, EDR_Right, EDR_Left, EDR_Back
};