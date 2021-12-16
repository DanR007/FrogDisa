// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveWithPuzzlesComponent.h"
#include "Movement.h"
#include "PuzzlePyatnashky.h"

#include "FrogDisa/TestPuzzleActor.h"
#include "FrogDisa/RotatablePuzzleActor.h"


#define ECC_PuzzleActorTraceChannel ECC_GameTraceChannel4

AMovement* OwnerPlayer;
// Sets default values for this component's properties
UInteractiveWithPuzzlesComponent::UInteractiveWithPuzzlesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractiveWithPuzzlesComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerPlayer = Cast<AMovement>(GetOwner());
	// ...
	
}


// Called every frame
void UInteractiveWithPuzzlesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractiveWithPuzzlesComponent::ActionWithPuzzleActor()
{
	if (OwnerPlayer->CanMakeAction())
	{
		
		FHitResult hitPoint;
		FVector Start = OwnerPlayer->_Camera->GetComponentLocation();
		FVector End = OwnerPlayer->_Camera->GetForwardVector() * 1000.f + Start;
		if (GetWorld()->LineTraceSingleByChannel(hitPoint, Start, End, ECC_PuzzleActorTraceChannel))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "NICE YOU COMPLETE THIS!");
			if (IsPuzzleTypeWithoutSpecialView(hitPoint.Actor.Get()))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "NICE YOU COMPLETE THIS!");
				Cast<ARotatablePuzzleActor>(hitPoint.Actor.Get())->ChangeRotation(Cast<UStaticMeshComponent>(hitPoint.GetComponent()));
			}
			else
			{
				Cast<APuzzleActor>(hitPoint.Actor.Get())->Use();
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