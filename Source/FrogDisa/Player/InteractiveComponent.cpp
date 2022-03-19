// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveComponent.h"

#include "FrogDisa/PuzzleActors/PuzzleInteractiveObject.h"

#include "DrawDebugHelpers.h"

#include "FrogDisa/DefaultVariables.h"
#define ECC_InteractiveObjectTraceChannel ECC_GameTraceChannel3

FCollisionQueryParams colQueryParams;
IInteractiveObjectsInterface* InteractiveInterfaceActor;
ICarriedObjectLogicInterface* CarriedActor;

// Sets default values for this component's properties
UInteractiveComponent::UInteractiveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UInteractiveComponent::BeginPlay()
{
	Super::BeginPlay();
	colQueryParams.AddIgnoredActor(PlayerActor);
}

bool UInteractiveComponent::TakeInteractiveObject()
{
	UCameraComponent* CameraOwner = PlayerActor->Camera;

	if (CarriedActor)
	{
		if (InteractiveActor)
		{
			InteractiveMesh = InteractiveActor->FindComponentByClass<UStaticMeshComponent>();
			if (InteractiveMesh)
			{
				InteractiveMesh->SetSimulatePhysics(false);//if simulate physics == true character can't move
				InteractiveMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				InteractiveMesh->SetCollisionResponseToChannel(ECC_InteractiveObjectTraceChannel, ECollisionResponse::ECR_Ignore);
			}

			
			InteractiveActor->SetActorLocation(CameraOwner->GetComponentLocation() + CameraOwner->GetForwardVector() * DistanceInteractiveObject);
			InteractiveActor->SetActorRotation(CameraOwner->GetComponentRotation());
			InteractiveActor->AttachToActor(PlayerActor, FAttachmentTransformRules::KeepWorldTransform);

			InteractiveInterfaceActor->Execute_ChangeOutlines(InteractiveActor, false);
		}
	}
	
	return InteractiveMesh != nullptr;

}

void UInteractiveComponent::DropInteractiveObject(float ImpulseValue)
{
	InteractiveActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	InteractiveMesh->SetSimulatePhysics(true);
	InteractiveMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	InteractiveMesh->AddImpulse(PlayerActor->Camera->GetForwardVector() * ImpulseValue * InteractiveMesh->GetMass());
	InteractiveMesh->SetCollisionResponseToChannel(ECC_InteractiveObjectTraceChannel, ECollisionResponse::ECR_Block);

	InteractiveMesh = nullptr;
	InteractiveActor = nullptr;
}

bool UInteractiveComponent::OverlapOnlyInteractivePuzzle()
{
	TArray<AActor*> overlappingActors;
	InteractiveMesh->GetOverlappingActors(overlappingActors);
	bool overlapOnlyInteractivePuzzleOrNothing = true;
	for (AActor* actor : overlappingActors)
	{
		if (Cast<APuzzleCarriedObject>(actor) == nullptr)
		{
			overlapOnlyInteractivePuzzleOrNothing = false;
			break;
		}
	}

	return overlapOnlyInteractivePuzzleOrNothing;
}

void UInteractiveComponent::DetachInteractiveFromParent()
{	
	if(InteractiveActor)
		InteractiveActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	if(InteractiveMesh)
		InteractiveMesh->SetCollisionResponseToChannel(ECC_InteractiveObjectTraceChannel, ECollisionResponse::ECR_Ignore);
	InteractiveMesh = nullptr;
	InteractiveActor = nullptr;
}

void UInteractiveComponent::CheckInteractiveObject()
{
	if (PlayerActor && PlayerActor->Camera)
	{
		FHitResult hitPoint;

		FVector Start = PlayerActor->Camera->GetComponentLocation();
		FVector End = PlayerActor->Camera->GetForwardVector() * MaximumCollectibleObjectDistance + Start;
		if (GetWorld()->LineTraceSingleByChannel(hitPoint, Start, End, ECC_InteractiveObjectTraceChannel, colQueryParams))
		{
			if (Cast<IInteractiveObjectsInterface>(hitPoint.Actor.Get()))
			{
				InteractiveInterfaceActor = Cast<IInteractiveObjectsInterface>(hitPoint.Actor.Get());
				if (InteractiveActor == nullptr)
				{
					InteractiveActor = hitPoint.Actor.Get();
					if (InteractiveActor)
						InteractiveInterfaceActor->Execute_ChangeOutlines(InteractiveActor, true);//Execute нужен чтобы вызвать функцию интерфейса в нужном объекте
				}
				if (Cast<ICarriedObjectLogicInterface>(hitPoint.Actor.Get()))
				{
					CarriedActor = Cast<ICarriedObjectLogicInterface>(hitPoint.Actor.Get());
				}

			}
			else
			{
				SetNullInteractiveObject();
			}
		}
		else
		{
			SetNullInteractiveObject();
		}
	}
}

void UInteractiveComponent::SetNullInteractiveObject()
{
	if (InteractiveActor)
	{
		if (InteractiveInterfaceActor)
			InteractiveInterfaceActor->Execute_ChangeOutlines(InteractiveActor, false);
		InteractiveActor = nullptr;
		InteractiveInterfaceActor = nullptr;
		CarriedActor = nullptr;
	}
}
