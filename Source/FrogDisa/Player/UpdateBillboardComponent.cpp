// Fill out your copyright notice in the Description page of Project Settings.


#include "UpdateBillboardComponent.h"
#include "Movement.h"
#include "FrogDisa/ObjectTakenInterface.h"
#include "Materials/Material.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/EngineTypes.h"

#define ECC_GrapplingObjectTraceChannel ECC_GameTraceChannel2
#define ECC_CollectiblesObjectTraceChannel ECC_GameTraceChannel3
// Sets default values for this component's properties

AMovement* Owner;
FCollisionQueryParams colQueryParams;
IObjectTakenInterface* TakenActor;



UUpdateBillboardComponent::UUpdateBillboardComponent()
{

	// ...
}

// Called when the game starts
void UUpdateBillboardComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<AMovement>(GetOwner());
	colQueryParams.AddIgnoredActor(Owner);
	// ...
	
}

void UUpdateBillboardComponent::CheckGrapplingPoint(bool isGrappling, FVector& HitLocation)
{
	if (Owner)
	{
		FHitResult hitPoint;

		FVector Start = Owner->Camera->GetComponentLocation();
		FVector End = Owner->Camera->GetForwardVector() * 3000.f + Start;

		

		if (GetWorld()->LineTraceSingleByChannel(hitPoint, Start, End, ECC_GrapplingObjectTraceChannel, colQueryParams))
		{
			if (Cast<AGrapplingObject>(hitPoint.Actor.Get()) && Owner->GetDistanceTo(Cast<AGrapplingObject>(hitPoint.Actor.Get())) > MinimumGrapplingDistance
				&& Owner->GetDistanceTo(Cast<AGrapplingObject>(hitPoint.Actor.Get())) < MaximumGrapplingDistance)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "Grappling is block");
				ActorGrapplingPoint = Cast<AGrapplingObject>(hitPoint.Actor.Get());
				HitLocation = hitPoint.Location;
				ActorGrapplingPoint->SetActiveObject(true);
			}
			else
			{
				if (isGrappling == false)
				{
					if (ActorGrapplingPoint)
					{
						ActorGrapplingPoint->SetActiveObject(false);
					}
					ActorGrapplingPoint = nullptr;
				}
			}
		}
		else
		{
			if (isGrappling == false)
			{
				if (ActorGrapplingPoint)
				{
					ActorGrapplingPoint->SetActiveObject(false);
				}
				ActorGrapplingPoint = nullptr;
			}
		}
	}
}

void UUpdateBillboardComponent::CheckCollectibleActor()
{
	if (Owner)
	{
		FHitResult hitPoint;

		FVector Start = Owner->Camera->GetComponentLocation();
		FVector End = Owner->Camera->GetForwardVector() * MaximumCollectibleObjectDistance + Start;

		if (GetWorld()->LineTraceSingleByChannel(hitPoint, Start, End, ECC_CollectiblesObjectTraceChannel, colQueryParams))
		{
			if (Cast<IObjectTakenInterface>(hitPoint.Actor.Get()))
			{
				TakenActor = Cast<IObjectTakenInterface>(hitPoint.Actor.Get());
				if (ActorTakenObject == nullptr)
				{
					ActorTakenObject = hitPoint.Actor.Get();
					if (ActorTakenObject)
						TakenActor->Execute_ChangeScalarParameter(ActorTakenObject, 1.f);
				}
			}
			else
			{
				if (TakenActor)
				{
					if (ActorTakenObject)
						TakenActor->Execute_ChangeScalarParameter(ActorTakenObject, 0.f);
					TakenActor = nullptr;
					ActorTakenObject = nullptr;
				}
			}
		}
		else
		{
			if (TakenActor)
			{
				if(ActorTakenObject)
					TakenActor->Execute_ChangeScalarParameter(ActorTakenObject, 0.f);
				TakenActor = nullptr;
				ActorTakenObject = nullptr;
			}
		}
	}
}