#include "GrapplingComponent.h"
#include "FrogDisa/DefaultVariables.h"
#include "GameFramework/CharacterMovementComponent.h"

#define ECC_GrapplingObjectTraceChannel ECC_GameTraceChannel2

// Sets default values for this component's properties
UGrapplingComponent::UGrapplingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrapplingComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


// Called every frame
void UGrapplingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UGrapplingComponent::StartGrappling()
{
	if (grappling_mode_active && can_grappling)
	{

		if (PlayerActor->isGrappling)
		{
			PlayerActor->isGrappling = false;
			PlayerActor->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
			PlayerActor->GetWorldTimerManager().ClearTimer(GrapplingTimer);
			PlayerActor->GetCharacterMovement()->Velocity = FVector::ZeroVector;
			PlayerActor->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
		else
			if (PlayerActor->HUDComponent && PlayerActor->HUDComponent->Stamina - 10.f > 0.f)
			{
				PlayerActor->HUDComponent->UpdateStamina(-10.f);

				PlayerActor->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
				PlayerActor->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				if (grappling_upper_object)
				{
					ChangeActiveGrapplingMode();
					PlayerActor->GetWorld()->GetTimerManager().SetTimer(GrapplingTimer, this, &UGrapplingComponent::LerpToUpperObject, 0.01f, true, 0.f);
				}
				else
				{
					PlayerActor->isGrappling = true;
					PlayerActor->GetWorld()->GetTimerManager().SetTimer(GrapplingTimer, this, &UGrapplingComponent::LerpTo, 0.01f, true, 0.f);
				}
			}

	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Grappling mode inactive"))
	}
}

void UGrapplingComponent::ChangeActiveGrapplingMode()
{ 
	grappling_mode_active = !grappling_mode_active;
	if (grappling_mode_active && PlayerActor)
	{
		PlayerActor->DrawGrapplingVariant();
	}
}

void UGrapplingComponent::LerpTo()
{
	if (PlayerActor->isGrappling &&
		PlayerActor->HUDComponent->Stamina - 0.2f <= 0)
	{
		PlayerActor->isGrappling = false;
		PlayerActor->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		//UE_LOG(LogTemp, Warning, TEXT("Stop Grappling"))
		PlayerActor->GetWorldTimerManager().ClearTimer(GrapplingTimer);
		PlayerActor->GetMovementComponent()->Velocity = FVector::ZeroVector;
		PlayerActor->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		PlayerActor->HUDComponent->UpdateStamina(-0.2f);

		PlayerActor->GetMovementComponent()->Velocity = (grappling_target_location - PlayerActor->GetActorLocation()) * 10;
	}
}

void UGrapplingComponent::LerpToUpperObject()//lerp while distance > 70
{
	if (FVector::Distance(PlayerActor->GetActorLocation(), grappling_target_location) <= PlayerActor->CapsuleRadius + 2.f)
	{
		PlayerActor->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		PlayerActor->GetWorldTimerManager().ClearTimer(GrapplingTimer);
		PlayerActor->GetMovementComponent()->Velocity = FVector::ZeroVector;
		PlayerActor->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ChangeActiveGrapplingMode();
	}
	else
	{
		PlayerActor->GetMovementComponent()->Velocity = (grappling_target_location - PlayerActor->GetActorLocation()) * 10;
	}
}