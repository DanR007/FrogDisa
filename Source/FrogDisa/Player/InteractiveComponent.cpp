// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveComponent.h"
#include "Movement.h"
#include "FrogDisa/PuzzleInteractiveObject.h"

// Sets default values for this component's properties
UInteractiveComponent::UInteractiveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractiveComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	// ...
	
}


// Called every frame
void UInteractiveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UInteractiveComponent::TakeInteractiveObject(UStaticMeshComponent* Player_InteractiveMesh)
{
	FHitResult hitPoint;

	FVector End = Owner->GetActorForwardVector() * 500.f + Owner->GetActorLocation();
	//DrawDebugLine(GetWorld(), GetActorLocation(), End, FColor::Blue, false, 10.f, 5);
	if (Owner->GetWorld()->LineTraceSingleByChannel(hitPoint, Owner->GetActorLocation(), End, ECC_Visibility, CollisionParams) == true)
	{
		if (hitPoint.Actor->IsA(AInteractiveObject::StaticClass()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Take this"));

			InteractiveActor = hitPoint.Actor.Get();
			InteractiveActor->SetActorLocation(Owner->GetActorLocation() + Owner->GetActorForwardVector() * 90.f + Owner -> GetActorUpVector() * 10.f);
			InteractiveActor->SetActorRotation(Owner->GetActorRotation());
			InteractiveMesh = InteractiveActor->FindComponentByClass<UStaticMeshComponent>();
			InteractiveMesh->SetSimulatePhysics(false);
			InteractiveMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			InteractiveActor->AttachToActor(Owner, FAttachmentTransformRules::KeepWorldTransform);
			Player_InteractiveMesh = InteractiveMesh;
		}
	}
	
	return InteractiveMesh != nullptr;

}

void UInteractiveComponent::DropInteractiveObject(UStaticMeshComponent* Player_InteractiveMesh)
{
	InteractiveActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	InteractiveMesh->SetSimulatePhysics(true);
	InteractiveMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	InteractiveMesh = nullptr;
	Player_InteractiveMesh = InteractiveMesh;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Drop this"));
}

void UInteractiveComponent::ThrowInteractiveObject(UStaticMeshComponent* Player_InteractiveMesh)
{

	InteractiveActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	InteractiveMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	InteractiveMesh->SetSimulatePhysics(true);
	InteractiveMesh->AddImpulse(Cast<AMovement>(Owner)->_Camera->GetForwardVector() * 1500.f * InteractiveMesh->GetMass());
	InteractiveMesh = nullptr;
	Player_InteractiveMesh = InteractiveMesh;
}

bool UInteractiveComponent::IsZeroOverlappingActors()
{
	TArray<AActor*> overlappingActors;
	InteractiveMesh->GetOverlappingActors(overlappingActors);

	return overlappingActors.Num() == 0;
}

bool UInteractiveComponent::OverlapOnlyInteractivePuzzle()
{
	TArray<AActor*> overlappingActors;
	InteractiveMesh->GetOverlappingActors(overlappingActors);
	bool overlapOnlyInteractivePuzzleOrNothing = true;
	for (AActor* actor : overlappingActors)
	{
		if (Cast<APuzzleInteractiveObject>(actor) == nullptr)
		{
			overlapOnlyInteractivePuzzleOrNothing = false;
			break;
		}
	}

	return overlapOnlyInteractivePuzzleOrNothing;
}