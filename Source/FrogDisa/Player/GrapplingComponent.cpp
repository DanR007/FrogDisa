#include "GrapplingComponent.h"
#include"Movement.h"

#define ECC_GrapplingObjectTraceChannel ECC_GameTraceChannel2

AMovement* Player_Character;
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
	Player_Character = Cast<AMovement>(GetOwner());
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
		//UE_LOG(LogTemp, Warning, TEXT("Start grappling"))
		Player_Character->UseGrapplingHook();
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Grappling mode inactive"))
	}
}

void UGrapplingComponent::GrapplingToTarget()
{
	Player_Character->LerpTo();
}

void UGrapplingComponent::ChangeActiveGrapplingMode()
{ 
	grappling_mode_active = !grappling_mode_active;
	if (grappling_mode_active && Player_Character)
	{
		Player_Character->DrawGrapplingVariant();
	}
}
