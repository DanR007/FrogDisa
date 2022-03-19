// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveWithPuzzlesComponent.h"

#include "FrogDisa/PuzzleActors/PuzzlePyatnashky.h"
#include "FrogDisa/PuzzleActors/TestPuzzleActor.h"
#include "FrogDisa/PuzzleActors/RotatablePuzzleActor.h"

#include "FrogDisa/DefaultVariables.h"

#include "DrawDebugHelpers.h"

#define ECC_PuzzleActorTraceChannel ECC_GameTraceChannel4

AMovement* OwnerPlayer;
FCollisionQueryParams queryParams;

// Sets default values for this component's properties
UInteractiveWithPuzzlesComponent::UInteractiveWithPuzzlesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UInteractiveWithPuzzlesComponent::BeginPlay()
{
	Super::BeginPlay();
	queryParams.AddIgnoredActor(PlayerActor);
	// ...
	
}


void UInteractiveWithPuzzlesComponent::ActionWithPuzzleActor()
{
	if (PlayerActor->CanMakeAction())
	{
		
		FHitResult hitPoint;
		FVector Start = PlayerActor->Camera->GetComponentLocation();
		FVector End = PlayerActor->Camera->GetForwardVector() * 1000.f + Start;


		if (GetWorld()->LineTraceSingleByChannel(hitPoint, Start, End, ECC_GameTraceChannel5, queryParams))
		{
			if (Cast<APuzzleActor>(hitPoint.Actor.Get()) || Cast<ARotatablePuzzleActor>(hitPoint.Actor.Get()))
				if (IsPuzzleTypeWithoutSpecialView(hitPoint.Actor.Get()))
				{
					Cast<ARotatablePuzzleActor>(hitPoint.Actor.Get())->ChangeRotation(Cast<UStaticMeshComponent>(hitPoint.GetComponent()));
				}
				else
				{
					//if (Cast<APuzzlePyatnashky>(hitPoint.Actor.Get()))
					//{
					//	Cast<APuzzlePyatnashky>(hitPoint.Actor.Get()) -> 
					//}
					//else
					//{
					Cast<APuzzleActor>(hitPoint.Actor.Get())->Use();
					//}
				}
		}
	}
}


bool UInteractiveWithPuzzlesComponent::IsPuzzleTypeWithoutSpecialView(AActor* puzzleActor)
{
	if (Cast<APuzzleActor>(puzzleActor))
	{
		return false;
	}
	else
	{
		return true;
	}
}