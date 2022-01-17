#include "BasicWeaponActor.h"

// Sets default values
ABasicWeaponActor::ABasicWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABasicWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasicWeaponActor::Create()
{

}

void ABasicWeaponActor::Launch()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Launch");
}