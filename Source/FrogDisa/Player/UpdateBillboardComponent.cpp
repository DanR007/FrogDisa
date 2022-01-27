// Fill out your copyright notice in the Description page of Project Settings.


#include "UpdateBillboardComponent.h"
#include "Movement.h"
#include "FrogDisa/InteractiveObjectsInterface.h"
#include "Materials/Material.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/EngineTypes.h"

#define ECC_GrapplingObjectTraceChannel ECC_GameTraceChannel2
#define ECC_CollectiblesObjectTraceChannel ECC_GameTraceChannel3
// Sets default values for this component's properties

AMovement* Owner;
FCollisionQueryParams colQueryParams;
IInteractiveObjectsInterface* TakenActor;



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

void UUpdateBillboardComponent::CheckTakenObject()
{
	if (Owner)
	{
		FHitResult hitPoint;

		FVector Start = Owner->Camera->GetComponentLocation();
		FVector End = Owner->Camera->GetForwardVector() * MaximumCollectibleObjectDistance + Start;

		if (GetWorld()->LineTraceSingleByChannel(hitPoint, Start, End, ECC_CollectiblesObjectTraceChannel, colQueryParams))
		{
			if (Cast<IInteractiveObjectsInterface>(hitPoint.Actor.Get()))
			{
				TakenActor = Cast<IInteractiveObjectsInterface>(hitPoint.Actor.Get());
				if (ActorTakenObject == nullptr)
				{
					ActorTakenObject = hitPoint.Actor.Get();
					if (ActorTakenObject)
						TakenActor->Execute_ChangeScalarParameter(ActorTakenObject, 1.f);//Execute нужен чтобы вызвать функцию интерфейса в нужном объекте
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