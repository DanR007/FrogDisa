// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "BrokenWithoutInteractiveActor.generated.h"

UCLASS()
class FROGDISA_API ABrokenWithoutInteractiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABrokenWithoutInteractiveActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* MainMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USphereComponent* ColliderToCheckInteractiveObject;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* RotateMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool isBroken;
	UFUNCTION(BlueprintCallable)
	void CheckOverlappingActor(AActor* OtherActor);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
		FString Need_Name;

};
