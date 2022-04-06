// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCAIController.h"

ANPCAIController::ANPCAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ANPCAIController::NewPerception);
}

void ANPCAIController::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("SPAWN ANPCAIController"));
}

void ANPCAIController::Tick(float DeltaTime)
{

}

void ANPCAIController::NewPerception(AActor* NewActorPerception, FAIStimulus stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("Sight Triggered"));
}

