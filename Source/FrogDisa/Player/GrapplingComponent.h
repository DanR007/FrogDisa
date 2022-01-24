// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector grappling_target_location;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool GetGrapplingModeActive() { return grappling_mode_active; }

	void ChangeActiveGrapplingMode() { grappling_mode_active = !grappling_mode_active; DrawPath(grappling_mode_active); }

	void StartGrappling();
	void GrapplingToTarget();

	UFUNCTION(BlueprintNativeEvent)
		void DrawPath(bool f_grappling_mode_active);

private:
	
	bool grappling_mode_active;
	
};
