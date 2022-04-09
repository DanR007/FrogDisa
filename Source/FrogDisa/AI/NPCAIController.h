// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"


#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "FrogDisa/DefaultVariables.h"
#include "FrogDisa/Player/EProjectiles.h"

#include "NPCAIController.generated.h"

/**
 * 
 */
UCLASS()
class FROGDISA_API ANPCAIController : public AAIController
{
	GENERATED_BODY()

	ANPCAIController();

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	
	UAISenseConfig_Sight* SightSense;

	TArray<AActor*> actors_in_sight;
	AActor* PlayerInSight;

	AActor* ControlledActor;

public:
	void SetOwnerActor(AActor* own_actor) { ControlledActor = own_actor; }

protected:
	UFUNCTION()
	void NewPerception(AActor* NewActorPerception, FAIStimulus stimulus);

	void CalculateAlarmScale(float deltaTime);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBlackboardComponent* BlackboardComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBehaviorTreeComponent* BehaviorTreeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BlackboardKeys")
		FName ActorTargetKey = "TriggerActor";
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BlackboardKeys")
		FName LocationTargetKey = "TargetLocation";
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BlackboardKeys")
		FName CurrentConditionKey = "CurrentCondition";

private:
	const float SightSenseRadius = 1500.f;
	const float SightAngle = 45.f;

	const float MeleeSightDistance = SightSenseRadius / 2;

	const float MaxAlarmScale = 100.f;
	float AlarmScale = 0;

	const float MaxSearchingTimeScale = MaxAlarmScale * 1.5f;
	float SearchingTimeScale = 0;

	EAICondition current_condition = EAICondition::EAIC_Idle;

	bool isSeeTrigger;
	bool isWarning = false;
};
