#include "GrapplingComponent.h"
#include "FrogDisa/DefaultVariables.h"

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
	if(PlayerActor == nullptr)
		PlayerActor = Cast<AMovement>(GetOwner());
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
		PlayerActor->UseGrapplingHook();
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Grappling mode inactive"))
	}
}

void UGrapplingComponent::GrapplingToTarget()
{
	PlayerActor->LerpTo();
}

void UGrapplingComponent::ChangeActiveGrapplingMode()
{ 
	grappling_mode_active = !grappling_mode_active;
	if (grappling_mode_active && PlayerActor)
	{
		PlayerActor->DrawGrapplingVariant();
	}
}
