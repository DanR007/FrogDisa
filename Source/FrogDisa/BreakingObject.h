// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Player/ThrowProjectile.h"
#include "Player/Movement.h"
#include "BreakingObject.generated.h"

UCLASS()
class FROGDISA_API ABreakingObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakingObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* MeshConstruction1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* MeshConstruction2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* MeshConstruction3;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* MeshConstruction4;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* MeshConstruction5;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* MeshConstruction6;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* MeshConstruction7;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* BoxCollider;
	UFUNCTION(BlueprintNativeEvent)
		void OnOverlapMesh(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

 //{ MeshConstruction1, MeshConstruction2, MeshConstruction3, MeshConstruction4, MeshConstruction5, MeshConstruction6, MeshConstruction7 };
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	AThrowProjectile* WrenchOverlap;
	void BreakMesh(AThrowProjectile* Wrench);
	TSubclassOf<AThrowProjectile> WrenchClass;
	TArray<UStaticMeshComponent*> MeshArray;

	float time_Before_Destroy;

	FTimerHandle DestroyTimer;
	void DestroyAllParts();
};
