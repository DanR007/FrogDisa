// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"

#include "FrogDisa/InteractiveObjectsInterface.h"

#include "PatrolVariety.h"
#include "PointOfPatrol.h"
#include "NPCAIController.h"

#include "Components/WidgetComponent.h"

#include "BehaviorTree/BehaviorTree.h"

#include "NPCPawn.generated.h"

UCLASS()
class FROGDISA_API ANPCPawn : public ACharacter, public IInteractiveObjectsInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ANPCPawn();

	void Interact() override;

	void ChangeOutlines_Implementation(bool isOutline) override { UE_LOG(LogTemp, Warning, TEXT("C++")) }
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Die();

	UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }

	UFUNCTION(BlueprintCallable)
		TArray<APointOfPatrol*> GetPointsOfPatrol() { return patrol_path; }

	UFUNCTION(BlueprintCallable)
		EPatrolVariety GetVarietyOfPatrol() { return patrolVariety; }

	UFUNCTION(BlueprintCallable)
		int GetIndexOfPatrol() { return indexOfPatrol; }

	UFUNCTION(BlueprintCallable)
		void ChangeIndexOfPatrol(int next_index) { indexOfPatrol = next_index; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int patrolDirection = 1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//	USkeletalMeshComponent* skeletal_mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UWidgetComponent* widget_component;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		ANPCAIController* npc_controller;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere)
		TArray<APointOfPatrol*> patrol_path;

	UPROPERTY(EditAnywhere)
		EPatrolVariety patrolVariety;

	TSubclassOf<ANPCAIController> controllerSubclass;
	bool isDead = false;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool GetIsDead() { return isDead; }

private:

	int indexOfPatrol = 0;
};
