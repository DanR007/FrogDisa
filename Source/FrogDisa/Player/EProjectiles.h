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
	EW_Pistol UMETA(DisplayName = "EW_Pistol"),
	EW_Bug UMETA(DisplayName = "EW_Bug"),
	EW_None UMETA(DisplayName = "EW_None")
};

UENUM(BlueprintType)
enum class EObjectType : uint8
{
	EO_HelthUp UMETA(DisplayName = "EO_HelthUp"),
	EO_StaminaUp UMETA(DisplayName = "EO_StaminaUp"),
	EO_None UMETA(DisplayName = "EO_None")
};

UENUM(BlueprintType)
enum class EAICondition : uint8
{
	EAIC_Patrol UMETA(DisplayeName = "EAIC_Patrol"),
	EAIC_Angry UMETA(DisplayeName = "EAIC_Angry"),
	EAIC_Searching UMETA(DisplayeName = "EAIC_Searching"),
	EAIC_Panic UMETA(DisplayeName = "EAIC_Panic"),
	EAIC_Detection UMETA(DisplayeName = "EAIC_Detection"),
	EAIC_Idle UMETA(DisplayName = "EAIC_Idle"),
	EAIC_Warning UMETA(DisplayName = "EAIC_Warning"),
	EAIC_None UMETA(DisplayName = "EAIC_None")

};
//UENUM()
enum ERotatorDirection
{
	ERD_Forward, EDR_Right, EDR_Left, EDR_Back
};