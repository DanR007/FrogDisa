// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveActor.h"
#include "InteractiveObjectsInterface.h"
#include "DoorLockActor.generated.h"

UCLASS()
class FROGDISA_API ADoorLockActor : public AInteractiveActor, public IInteractiveObjectsInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorLockActor();

	void Interact() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Pivot;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* MainDoorMesh;

	UFUNCTION(BlueprintNativeEvent)
		void HackDoor();

	UFUNCTION(BlueprintCallable)
		void LockIsHack();

	UFUNCTION(BlueprintNativeEvent)
		void OpenOrClose();

	virtual EInteractionTypes GetInteractionType() const override { return interactive_type; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isLock;
};
