// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCAIController.h"
#include "NPCPawn.h"

#include "Kismet/KismetMathLibrary.h"

ANPCAIController::ANPCAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ANPCAIController::NewPerception);

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));

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
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("SPAWN ANPCAIController"));
	
}

void ANPCAIController::StartBehaviorTreeFromParent()
{
	ANPCPawn* CtrlPawn = Cast<ANPCPawn>(ControlledActor);
	if (CtrlPawn && CtrlPawn->GetBehaviorTree()->BlackboardAsset)
	{
		BlackboardComp->InitializeBlackboard(*CtrlPawn->GetBehaviorTree()->BlackboardAsset);
		BehaviorTreeComp->StartTree(*CtrlPawn->GetBehaviorTree());
	}
}

void ANPCAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerInSight)
	{
		ControlWhenPlayerInSight();
	}
	else
	{
		ControlWhenPlayerNotInSight();
	}

	BlackboardComp->SetValueAsEnum(CurrentConditionKey, (uint8)current_condition);
}

void ANPCAIController::ControlWhenPlayerInSight()
{
	if (isWarning && current_condition == EAICondition::EAIC_Searching)
	{
		AlarmScale = MaxAlarmScale;	
	}
	else
	{
		CalculateAlarmScale(DefaultDeltaTime);
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
			BlackboardComp->SetValueAsVector(LocationTargetKey, PlayerInSight->GetActorLocation());
		}
		else
		{
			current_condition = EAICondition::EAIC_Detection;
		}
	}
}

void ANPCAIController::ControlWhenPlayerNotInSight()
{
	if (current_condition == EAICondition::EAIC_Angry || current_condition == EAICondition::EAIC_Warning
		|| current_condition == EAICondition::EAIC_Searching)
	{
		current_condition = EAICondition::EAIC_Searching;
		SearchingTimeScale = UKismetMathLibrary::FClamp(SearchingTimeScale - DefaultDeltaTime * 2, 0, MaxSearchingTimeScale);
		if (SearchingTimeScale == 0)
		{
			current_condition = EAICondition::EAIC_Idle;
		}
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, "Search");
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::SanitizeFloat(SearchingTimeScale));
	}
	else
	{
		if (current_condition == EAICondition::EAIC_Detection)
		{
			AlarmScale = UKismetMathLibrary::FClamp(AlarmScale - DefaultDeltaTime * 2, 0, MaxAlarmScale);

			if (AlarmScale == 0)
			{
				current_condition = EAICondition::EAIC_Idle;
			}
		}
	}
}

void ANPCAIController::NewPerception(AActor* NewActorPerception, FAIStimulus stimulus)
{
	//UE_LOG(LogTemp, Warning, TEXT("Sight Triggered"));
	ANPCPawn* dead_pawn = Cast<ANPCPawn>(NewActorPerception);
	
	if (stimulus.WasSuccessfullySensed())
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
			DeadBodyLocation = dead_pawn->GetActorLocation();

			BlackboardComp->SetValueAsVector(LocationTargetKey, DeadBodyLocation);
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
	float scale = 2, current_distance = FVector::Distance(PlayerActor->GetActorLocation(), ControlledActor->GetActorLocation());

	if (isWarning)
		scale++;
	if (PlayerActor->GetCrouchMode() == false)
		scale++;
	if (current_distance <= MeleeSightDistance)
		scale += MeleeSightDistance / current_distance;

	AlarmScale = UKismetMathLibrary::FClamp(AlarmScale + deltaTime * scale, 0, MaxAlarmScale);
}

