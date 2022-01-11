// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatablePuzzleActor.h"
#include <vector>


std::vector<UStaticMeshComponent*> meshArray;
TArray<UStaticMeshComponent*> meshSet;
std::vector<FRotator> directionInDegrees = {FRotator(0,0,0), FRotator(0,90,0) ,FRotator(0,180,0) ,FRotator(0,270,0) };
std::vector<FRotator> g_correct_direction = { FRotator(0,90,0), FRotator(0,90,0), FRotator(0,90,0) };
std::vector<FRotator> g_direction = { FRotator(0,0,0),FRotator(0,0,0),FRotator(0,0,0) } ;
ARotatablePuzzleActor::ARotatablePuzzleActor()
{
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	
	FirstMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FirstMesh"));
	SecondMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecondMesh"));
	ThirdMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ThirdMesh"));

	RootComponent = MainMesh;
	FirstMesh->SetupAttachment(RootComponent);
	SecondMesh->SetupAttachment(RootComponent);
	ThirdMesh->SetupAttachment(RootComponent);

	meshArray = { FirstMesh, SecondMesh,ThirdMesh };
}

void ARotatablePuzzleActor::BeginPlay()
{
	
}

void ARotatablePuzzleActor::ChangeRotation(UStaticMeshComponent* mesh)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, mesh->GetName());
	for (int j = 0; j < 3; j++)
	{

		if (mesh == meshArray[j])
		{
			for (int i = 0; i < 90; i++)
			{
				meshArray[j]->AddRelativeRotation(FRotator(0, 1, 0));
				g_direction[j] = FRotator(0, 90, 0);
			}

			break;
		}
	}

	if (g_direction == g_correct_direction)
		PuzzleComplete();
}

void ARotatablePuzzleActor::SetDirectionSettings()
{
	for (int i = 0; i < 3; i++)
	{
		//start_directionstart[i];
		meshArray[i]->SetRelativeRotation(/*start_direction[i]*/FRotator(0,0,0));
	}
}

void ARotatablePuzzleActor::PuzzleComplete()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "NICE YOU COMPLETE THIS!");
	for (UStaticMeshComponent* component : meshArray)
	{
		//component->
	}
}