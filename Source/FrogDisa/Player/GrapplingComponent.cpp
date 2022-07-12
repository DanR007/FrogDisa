#include "GrapplingComponent.h"
#include "FrogDisa/DefaultVariables.h"
#include "GameFramework/CharacterMovementComponent.h"

#include <cmath>

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
	if (PlayerActor && PlayerActor->GetCapsuleComponent())
		capsule_half_height = PlayerActor->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}


// Called every frame
void UGrapplingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	ChoiceGrapplingVariant();
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
					//PlayerActor->SetActorLocation(grappling_target_location);
					PlayerActor->GetWorld()->GetTimerManager().SetTimer(GrapplingTimer, this, &UGrapplingComponent::LerpToUpperObject, 0.01f, true, 0.f);
				}
				else
				{
					PlayerActor->isGrappling = true;
					//PlayerActor->SetActorLocation(grappling_target_location);
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
}

void UGrapplingComponent::LerpTo()
{
	if (PlayerActor->isGrappling &&
		PlayerActor->HUDComponent->Stamina - 0.2f <= 0)
	{
		PlayerActor->isGrappling = false;
		PlayerActor->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		//UE_LOG(LogTemp, Warning, TEXT("Stop Grappling"))
		StopGrappling();
	}
	else
	{
		PlayerActor->HUDComponent->UpdateStamina(-0.02f);

		PlayerActor->GetMovementComponent()->Velocity = (grappling_target_location - PlayerActor->GetActorLocation()) * 10;
	}
}

void UGrapplingComponent::LerpToUpperObject()//lerp while distance > 70
{
	if (FVector::Distance(PlayerActor->GetActorLocation(), grappling_target_location) <= PlayerActor->DefaultCapsuleRadius + 2.f)
	{
		PlayerActor->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		StopGrappling();
		ChangeActiveGrapplingMode();
	}
	else
	{
		PlayerActor->GetMovementComponent()->Velocity = (grappling_target_location - PlayerActor->GetActorLocation()) * 10;
	}
}

void UGrapplingComponent::ChoiceGrapplingVariant()
{
	if (grappling_mode_active && !PlayerActor->isGrappling)
	{
		FHitResult hitResult;

		FVector startLoc = PlayerActor->Camera->GetComponentLocation();
		SetCanGrappling(CastLineTrace(startLoc,
			startLoc + PlayerActor->Camera->GetForwardVector() * max_grapppling_distance
			, hitResult));

		if (GetCanGrappling())
		{
			hit_location = hitResult.Location;
			AActor* hitActor = hitResult.GetActor();
			FVector endLoc = hit_location + PlayerActor->GetActorForwardVector() * -1, impactNormal = hitResult.ImpactNormal;
			bool upperHit = CastLineTrace(endLoc + PlayerActor->GetActorUpVector() * (capsule_half_height - 5), endLoc, hitResult);
			if (hitActor)
				if (hit_location.Z < hitActor->GetActorLocation().Z)
				{
					SetGrapplingTargetLocation(hit_location + FVector(0, 0, capsule_half_height + offset_grappling_height), true);
				}
				else
				{
					if (upperHit)
					{
						SetGrapplingTargetLocation(hit_location + FVector(0, 0, capsule_half_height + offset_grappling_height), false);
					}
					else
					{
						float last_height = MAX_FLT;

						bool can_grappling_upper_obj = false;

						FVector last_grappling_pos;
						for (int i = 0; i < 10; i++)
						{
							FHitResult lastHitRes;
							if (CastLineTrace(hit_location + FVector(0, 0, 50.f) + PlayerActor->GetActorForwardVector() * i, hit_location + PlayerActor->GetActorForwardVector() * i, lastHitRes))
							{
								if (lastHitRes.Distance > 0.1f)
								{
									CheckHeight(lastHitRes.Location, can_grappling_upper_obj, last_grappling_pos, last_height);
								}
							}
						}

						if (can_grappling_upper_obj)
							SetGrapplingTargetLocation(last_grappling_pos + FVector(0, 0, capsule_half_height + offset_grappling_height), true);
						else
							SetGrapplingTargetLocation(hit_location + impactNormal * PlayerActor->GetCapsuleComponent()->GetScaledCapsuleRadius(), false);
					}
				}

		}
	}
	
	hit_location = FVector::ZeroVector;
}

void UGrapplingComponent::StopGrappling()
{
	PlayerActor->GetWorldTimerManager().ClearTimer(GrapplingTimer);
	PlayerActor->GetMovementComponent()->Velocity = FVector::ZeroVector;
	PlayerActor->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

bool UGrapplingComponent::CastLineTrace(const FVector& startLoc, const FVector& endLoc, FHitResult& hitRes) const
{
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(PlayerActor);
	return GetWorld()->LineTraceSingleByChannel(hitRes, startLoc, endLoc,
		GRAPPLING_TRACE_CHANNEL, queryParams);
}

void UGrapplingComponent::CheckHeight(const FVector& hitLoc, bool& canUpper, FVector& lasHitLoc, float& lastHeight)
{
	FVector localHitPosition = hitLoc;

	if (localHitPosition.Z - hit_location.Z <= lastHeight)
	{
		canUpper = true;
		lasHitLoc = localHitPosition;
		lastHeight = std::min(lastHeight, localHitPosition.Z - hit_location.Z);
	}
	
}