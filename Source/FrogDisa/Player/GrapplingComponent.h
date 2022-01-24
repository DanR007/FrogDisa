// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "GrapplingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FROGDISA_API UGrapplingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrapplingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		bool GetGrapplingModeActive() { return grappling_mode_active; }

	void ChangeActiveGrapplingMode(); 

	void StartGrappling();
	void GrapplingToTarget();

	UFUNCTION(BlueprintCallable)
	void SetGrapplingTargetLocation(FVector location)
	{
		grappling_target_location = location;
	}
	UFUNCTION(BlueprintCallable)
		void SetCanGrappling(bool can) { can_grappling = can; }

	bool GetCanGrappling() { return can_grappling; }

	FTimerHandle GrapplingTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector grappling_target_location;
private:
	
	bool grappling_mode_active;
	bool can_grappling;

	
};
