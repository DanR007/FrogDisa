// Fill out your copyright notice in the Description page of Project Settings.


#include "SteamBug.h"

// Sets default values
ASteamBug::ASteamBug()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASteamBug::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASteamBug::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASteamBug::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASteamBug::SetMainCharacter(ACharacter*f_main_character)
{
	main_character = f_main_character;
	//PossessedBy(GetWorld()->GetFirstPlayerController());
	//ReceivePossessed(GetWorld()->GetFirstPlayerController());
}

void ASteamBug::SetNewPosses_Implementation()
{

}
