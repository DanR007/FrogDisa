// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FrogDisa/InteractiveObjectsInterface.h"
#include "NPCAIController.h"

#include "Components/WidgetComponent.h"

#include "BehaviorTree/BehaviorTree.h"

#include "NPCPawn.generated.h"

UCLASS()
class FROGDISA_API ANPCPawn : public APawn, public IInteractiveObjectsInterface
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
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* skeletal_mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		ANPCAIController* npc_controller;

	UPROPERTY(EditDefaultsOnly)
		UBehaviorTree* BehaviorTree;

	TSubclassOf<ANPCAIController> controllerSubclass;
	bool isDead = false;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool GetIsDead() { return isDead; }
};
