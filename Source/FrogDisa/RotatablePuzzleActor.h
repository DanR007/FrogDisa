// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzleActor.h"
#include "Player/EProjectiles.h"
#include "RotatablePuzzleActor.generated.h"

/**
 *
 */
UCLASS()
class FROGDISA_API ARotatablePuzzleActor : public AActor
{
	GENERATED_BODY()
public:
	ARotatablePuzzleActor();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Meshes")
		UStaticMeshComponent* FirstMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Meshes")
		UStaticMeshComponent* SecondMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Meshes")
		UStaticMeshComponent* ThirdMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FRotator> start_direction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FRotator> right_direction;

	void ChangeRotation(UStaticMeshComponent* mesh);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void SetDirectionSettings();

	
	void PuzzleComplete();
};

