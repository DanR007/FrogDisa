// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

enum EProjectiles
{
	EP_Stone, EP_Wrench, EP_Tranquilizer, EP_ThrowingKnife, EP_CrossbowBolt,
	EP_Flashbang, EP_Grenade, EP_SmokeGrenade, EP_
};

enum EWeaponType
{
	EW_Stone, EW_Tranquilizer, EW_ThrowingKnife, EW_CrossbowBolt,
	EW_Flashbang, EW_Grenade, EW_SmokeGrenade, EW_SmokeMine, EW_FlashbangMine,
	EW_Mine
};
//UENUM()
enum ERotatorDirection
{
	ERD_Forward, EDR_Right, EDR_Left, EDR_Back
};