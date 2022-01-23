// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveComponent.h"
#include "Movement.h"
#include "FrogDisa/PuzzleActors/PuzzleInteractiveObject.h"

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
	Owner = GetOwner();
	CollisionParams.AddIgnoredActor(Owner);
}

bool UInteractiveComponent::TakeInteractiveObject(UStaticMeshComponent* Player_InteractiveMesh)//������ ���������� ����
{
	FHitResult hitPoint;

	UCameraComponent* CameraOwner = Cast<AMovement>(Owner)->Camera;

	FVector Start = CameraOwner->GetComponentLocation();
	FVector End = Start + CameraOwner->GetForwardVector() * 500.f;

	if (Owner->GetWorld()->LineTraceSingleByChannel(hitPoint, Start, End, ECC_Visibility, CollisionParams) == true)
	{
		if (hitPoint.Actor->IsA(AInteractiveObject::StaticClass()))
		{
			InteractiveActor = hitPoint.Actor.Get();
			InteractiveActor->SetActorLocation(Start + CameraOwner->GetForwardVector() * 90.f);
			InteractiveActor->SetActorRotation(Owner->GetActorRotation());
			InteractiveMesh = InteractiveActor->FindComponentByClass<UStaticMeshComponent>();
			InteractiveMesh->SetSimulatePhysics(false);//if simulate physics == true character can't move
			InteractiveMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			InteractiveActor->AttachToActor(Owner, FAttachmentTransformRules::KeepWorldTransform);
			Player_InteractiveMesh = InteractiveMesh;
		}
	}
	
	return InteractiveMesh != nullptr;

}

void UInteractiveComponent::DropInteractiveObject(UStaticMeshComponent* Player_InteractiveMesh, float ImpulseValue)
{
	InteractiveActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	InteractiveMesh->SetSimulatePhysics(true);
	InteractiveMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	InteractiveMesh->AddImpulse(Cast<AMovement>(Owner)->Camera->GetForwardVector() * ImpulseValue * InteractiveMesh->GetMass());
	InteractiveMesh = nullptr;
	InteractiveActor = nullptr;
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

void UInteractiveComponent::DetachInteractiveFromParent(UStaticMeshComponent* Player_InteractiveMesh)
{	
	InteractiveActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	InteractiveMesh = nullptr;
	InteractiveActor = nullptr;
	Player_InteractiveMesh = nullptr;
}
