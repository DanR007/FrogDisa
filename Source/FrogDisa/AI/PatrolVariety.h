// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPatrolVariety: uint8
{
	PV_Do_Nothing UMETA(DisplayName = "PV_Do_Nothing"),
	PV_Loop UMETA(DisplayName = "PV_Loop"),
	PV_Patrol_Backward UMETA(DisplayName = "PV_Patrol_Backward")
};
