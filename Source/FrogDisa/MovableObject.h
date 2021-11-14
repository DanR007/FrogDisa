// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovableObject.generated.h"

UCLASS()
class FROGDISA_API AMovableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* MovableMesh;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetMovable();
	void SetStatic();
};
