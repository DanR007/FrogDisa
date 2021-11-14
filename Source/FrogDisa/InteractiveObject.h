// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveObjectComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "InteractiveObject.generated.h"


UCLASS()
class FROGDISA_API AInteractiveObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//UFUNCTION(BlueprintAssignable)
	void TakeThisObject();
	//UFUNCTION(BlueprintAssignable)
	void DropThisObject();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UInteractiveObjectComponent* Interactive;
};
