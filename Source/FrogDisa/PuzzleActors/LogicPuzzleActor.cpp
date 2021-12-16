// Fill out your copyright notice in the Description page of Project Settings.


#include "LogicPuzzleActor.h"
#include <vector>


std::vector<FString> nameComponents;
std::vector<bool> boolComponents;
// Sets default values
ALogicPuzzleActor::ALogicPuzzleActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BaseComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	RootComponent = BaseComponent;

	FirstComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("First"));
	SecondComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Second"));
	ThirdComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Third"));
	FourthComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Fourth"));
	FifthComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Fifth"));
	SixthComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sixth"));

	Components = { FirstComponent, SecondComponent, ThirdComponent, FourthComponent, FifthComponent,
	SixthComponent };

	boolComponents = { false,false,false,false,false,false };

	for (UStaticMeshComponent* mesh : Components)
	{
		mesh->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void ALogicPuzzleActor::BeginPlay()
{
	Super::BeginPlay();
	
}



// Called every frame
void ALogicPuzzleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALogicPuzzleActor::SetArrayComponents(TArray<FString> blueprint_arr)
{
	for (FString str : blueprint_arr)
	{
		nameComponents.push_back(str);
	}
}

bool ALogicPuzzleActor::IsObjectByIndexEqual(int index_array, FString name_component)
{
	if (nameComponents[index_array] == name_component)
	{
		boolComponents[index_array] = true;
	}

	return boolComponents[index_array];
}

bool ALogicPuzzleActor::IsObjectBeEqual(int index_array)
{
	return boolComponents[index_array];
}
