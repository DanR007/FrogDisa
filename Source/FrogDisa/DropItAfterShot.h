// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "CableComponent.h"
#include "Player/ThrowProjectile.h"
#include "UObject/ConstructorHelpers.h"
#include "DropItAfterShot.generated.h"

UCLASS()
class FROGDISA_API ADropItAfterShot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropItAfterShot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* MeshConstruction;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* BreakableMesh;
	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UCableComponent* CableMesh1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UCableComponent* CableMesh2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UCableComponent* CableMesh3;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UCableComponent* CableMesh4;*/
	UFUNCTION(BlueprintCallable)
		void DropConstruction();
	UFUNCTION(BlueprintCallable)
		void StopDropConstruction();
public:	
	// Called every frame
	UFUNCTION(BlueprintCallable)
		bool isABreakableMesh();
	UFUNCTION(BlueprintNativeEvent)
		void OnOverlapBreakableMesh(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	virtual void Tick(float DeltaTime) override;
private:
	TSubclassOf<AThrowProjectile> Wrench;
};
