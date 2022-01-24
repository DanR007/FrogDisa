#include "GrapplingComponent.h"

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

void UGrapplingComponent::DrawPath_Implementation(bool f_grappling_mode_active)
{

}

void UGrapplingComponent::StartGrappling()
{
	if (grappling_mode_active)
	{
		UE_LOG(LogTemp, Warning, TEXT("Start grappling"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Grappling mode inactive"))
	}
}

void UGrapplingComponent::GrapplingToTarget()
{

}

