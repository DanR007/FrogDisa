// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCAIController.h"
#include "NPCPawn.h"

#include "Kismet/KismetMathLibrary.h"

ANPCAIController::ANPCAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ANPCAIController::NewPerception);

	SightSense = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Sense"));

	SightSense->SightRadius = SightSenseRadius;
	SightSense->LoseSightRadius = SightSenseRadius + 50.f;
	SightSense->PeripheralVisionAngleDegrees = SightAngle;
	SightSense->SetMaxAge(1.f);//what doing this function
	SightSense->DetectionByAffiliation.bDetectEnemies = true;
	SightSense->DetectionByAffiliation.bDetectFriendlies = true;
	SightSense->DetectionByAffiliation.bDetectNeutrals = true;
	
	PerceptionComponent->SetDominantSense(SightSense->GetSenseImplementation());
	PerceptionComponent->ConfigureSense(*SightSense);
}

void ANPCAIController::BeginPlay()
{
	//UE_LOG(LogTemp, Warning, TEXT("SPAWN ANPCAIController"));
	
}

void ANPCAIController::Tick(float DeltaTime)
{
	if (PlayerInSight)
	{
		if (isWarning && current_condition == EAICondition::EAIC_Searching)
		{
			AlarmScale = MaxAlarmScale;
		}
		else
		{
			CalculateAlarmScale(DeltaTime);
		}

		
		if (AlarmScale == MaxAlarmScale)
		{
			current_condition = EAICondition::EAIC_Angry;
			SearchingTimeScale = MaxSearchingTimeScale;
			isWarning = true;
		}
		else 
		{
			if (AlarmScale >= MaxAlarmScale / 2)
			{
				current_condition = EAICondition::EAIC_Warning;
				SearchingTimeScale = MaxSearchingTimeScale / 2;
			}
			else
			{
				current_condition = EAICondition::EAIC_Detection;
			}
		}
	}
	else
	{
		if (current_condition == EAICondition::EAIC_Angry || current_condition == EAICondition::EAIC_Warning
			|| current_condition == EAICondition::EAIC_Searching)
		{
			current_condition = EAICondition::EAIC_Searching;
			SearchingTimeScale = UKismetMathLibrary::FClamp(SearchingTimeScale - DeltaTime * 2, 0, MaxSearchingTimeScale);
			if (SearchingTimeScale == 0)
			{
				current_condition = EAICondition::EAIC_Idle;
			}
		}
		else
		{
			if (current_condition == EAICondition::EAIC_Detection)
			{
				AlarmScale = UKismetMathLibrary::FClamp(AlarmScale - DeltaTime * 2, 0, MaxAlarmScale);

				if (AlarmScale == 0)
				{
					current_condition = EAICondition::EAIC_Idle;
				}
			}
		}

	}
}

void ANPCAIController::NewPerception(AActor* NewActorPerception, FAIStimulus stimulus)
{
	//UE_LOG(LogTemp, Warning, TEXT("Sight Triggered"));
	ANPCPawn* dead_pawn = Cast<ANPCPawn>(NewActorPerception);
	if (NewActorPerception == PlayerActor)
	{
		isSeeTrigger = true;
		PlayerInSight = NewActorPerception;
	}
	else
	{
		if (dead_pawn && dead_pawn->GetIsDead() && actors_in_sight.Find(dead_pawn) == INDEX_NONE)
		{
			actors_in_sight.Add(dead_pawn);
			isSeeTrigger = true;
			isWarning = true;
			current_condition = EAICondition::EAIC_Searching;
		}
		else
		{
			if (current_condition != EAICondition::EAIC_Angry && PlayerInSight == nullptr)
				isSeeTrigger = false;
		}

		PlayerInSight = nullptr;
	}
}

void ANPCAIController::CalculateAlarmScale(float deltaTime)
{
	float scale = 2;

	if (isWarning)
		scale++;
	if (PlayerActor->GetCrouchMode() == false)
		scale++;
	if (FVector::Distance(PlayerActor->GetActorLocation(), ControlledActor->GetActorLocation()) <= MeleeSightDistance)
		scale++;

	AlarmScale = UKismetMathLibrary::FClamp(AlarmScale + deltaTime * scale, 0, MaxAlarmScale);
}

