// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LogicPuzzleActor.generated.h"

UCLASS()
class FROGDISA_API ALogicPuzzleActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALogicPuzzleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* BaseComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* FirstComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* SecondComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* ThirdComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* FourthComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* FifthComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* SixthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<UStaticMeshComponent*> Components;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> NameComponents;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable)
		void SetArrayComponents(TArray<FString> blueprint_arr);

	UFUNCTION(BlueprintCallable)
		bool IsObjectByIndexEqual(int index_array, FString name_component);

	UFUNCTION(BlueprintCallable)
		bool IsObjectBeEqual(int index_array);
};
