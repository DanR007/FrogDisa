// Fill out your copyright notice in the Description page of Project Settings.


#include "TestPuzzleActor.h"

// Sets default values
ATestPuzzleActor::ATestPuzzleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestPuzzleActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestPuzzleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestPuzzleActor::Use_Implementation()
{

}