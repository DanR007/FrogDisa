#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TimelineComponent.h"
#include "Components/BoxComponent.h"
//#include "HealthComponent.h"
#include "FrogDisa/MovableObject.h"
#include "ShootComponent.h"
#include "FrogDisa/InteractiveObject.h"
#include "GameFramework/HUD.h"
#include "FrogDisa/CollectiblesObject.h"
#include "GameFramework/MovementComponent.h"
#include "FrogDisa/GrapplingObject.h"
#include "FrogDisa/Collectibles.h"
#include "EProjectiles.h"
#include "Movement.generated.h"

UCLASS()
class FROGDISA_API AMovement : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMovement();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool MeleeAttackIsActive;

		void SetUnwaitingState();
protected:
	// Called when the game starts or when spawned

	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void Attack();

	void StopShoot();

	//void Heal();

	void Jump();

	void StopJumping();

	void UseGrapplingHook();

	void Aim(float Value);
	
	void Run(float Value);

	//UFUNCTION(Blueprint)
	void InteractionWithObject();

	void TakeCollectibles();
	//void ResetJumpState();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void AddControllerYawInput(float Val);

	virtual void AddControllerPitchInput(float Val);
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ReturnWrench();



	void PauseMenu();
	UFUNCTION(BlueprintCallable)
		void SetMeleeAttackInactive();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool isClimbing;
public:

	UFUNCTION(BlueprintCallable)
		bool GetWaitingState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "meshComponent")
		UStaticMeshComponent* meshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		USpringArmComponent* cameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		UCameraComponent* _Camera;

	//Переменная для отображения здоровья
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
		UHealthComponent* Health;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shoot")
		UShootComponent* ShootComponent;

	UFUNCTION(BlueprintCallable)
		bool GetAimingState();
	UFUNCTION(BlueprintCallable)
		bool GetRunningState();
	UFUNCTION(BlueprintCallable)
		AActor* GetThrowProjectile();

	UFUNCTION(BlueprintCallable)
		bool GetPauseState();
	UFUNCTION(BlueprintCallable)
		void SetUnPause();
private:
	void ForwardTrace();
	void HeightTrace();
	void LerpToGrapplingPoint(FVector StartLocation, FVector EndLocation, float time);
	void LerpTo();
	void SwitchProjectile();

	FTimeline MyTimeline;
	float CurveFloatValue;
	float TimelineValue;
	FTimerHandle GrapplingTimer;

	bool isBearObject;
	bool isAiming;
	bool isRunning;
	bool isGrappling;
	bool circleMenuIsOpen;
	bool craftMenuOpen;
	bool isWaitingWrench;

	int Collectibles;
	int stoneCount;

	FVector cameraOffsetYPlus;
	FVector cameraOffsetYMinus;
	FVector startLoc, endLoc;
	FVector wallNormal, wallLocation;

	const float MinimumGrapplingDistance = 200.f;
	const float MaximumGrapplingDistance = 5000.f;
	const float MaximumCollectibleObject = 800.f;
	const float DefaultSpeed = 400.f, RunningSpeed = DefaultSpeed + 200.f;
	const float ChangeTargetArmSpeed = 20.f;

	EProjectiles g_Projectile_Type;

	FCollisionQueryParams CollisionParams;
	AActor* InteractiveActor;
	UStaticMeshComponent* InteractiveObject;
	AGrapplingObject* ActorGrapplingPoint;
	ACollectiblesObject* ActorCollectibleObject;
	AMovableObject* movableActor;
};
