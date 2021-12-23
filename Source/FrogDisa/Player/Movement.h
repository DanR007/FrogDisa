#pragma once

#include "CoreMinimal.h"

#include "EProjectiles.h"
#include "SteamBug.h"
#include "ShootComponent.h"
#include "InteractiveComponent.h"
#include "FrogDisaGameInstance.h"
#include "UpdateBillboardComponent.h"
#include "InteractiveWithPuzzlesComponent.h"

#include "FrogDisa/GrapplingObject.h"
#include "FrogDisa/CollectiblesObject.h"
#include "FrogDisa/MovableObject.h"
#include "FrogDisa/InteractiveObject.h"

#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/HUD.h"

#include "Components/TimelineComponent.h"
#include "Components/BoxComponent.h"

#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"

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
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Action Functions default
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Attack();
	void Jump();
	void StopJumping();
	virtual void AddControllerYawInput(float Val);
	virtual void AddControllerPitchInput(float Val);

	void UseGrapplingHook();

	void Aim(float Value);
	
	void Run(float Value);

	void InteractionWithObject();
	void TakeCollectibles();
	bool CanMakeAction();

	void ChangeCharacter();

	void PauseMenu();

	UFUNCTION(BlueprintCallable)
		void SetMeleeAttackInactive();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool isClimbing;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetNewPosses();

	UUpdateBillboardComponent* UpdateGrapplingOrCollectibleActors;
public:

	void DetachInteractiveObject();

	UFUNCTION(BlueprintCallable)
		bool GetWaitingState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "meshComponent")
		UStaticMeshComponent* meshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		USpringArmComponent* cameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		UCameraComponent* _Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shoot")
		UShootComponent* ShootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive")
		UInteractiveComponent* InteractiveComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UFrogDisaGameInstance* game_Instance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UInteractiveWithPuzzlesComponent* InteractiveWithPuzzlesComponent;

	UFUNCTION(BlueprintCallable)
		void SetStartSettings(int countStone, int countCollectibles, bool isHaveBug, bool isBearObj);
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

	UFUNCTION(BlueprintCallable)
		bool IsHaveASteamBug();
	UFUNCTION(BlueprintCallable)
		bool IsBearObject();

	UFUNCTION(BlueprintCallable)
		int GetCountStones();

	UFUNCTION(BlueprintCallable)
		int GetCountCollectibles();
private:
	void ForwardTrace();
	void HeightTrace();
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
	bool pauseMenuOpen;
	bool isWaitingWrench;
	bool HaveSteamBug;

	int Collectibles;
	int stoneCount;

	FVector endLoc;
	FVector wallNormal, wallLocation;

	const float DefaultSpeed = 400.f, RunningSpeed = DefaultSpeed + 200.f;
	const float ChangeTargetArmSpeed = 20.f;
	const float ThrowImpulseValue = 1500.f;
	const float DropImpulseValue = 0;

	EProjectiles g_Projectile_Type;

	FCollisionQueryParams CollisionParams;
	UStaticMeshComponent* InteractiveObject;
	AMovableObject* movableActor;
	ASteamBug* SteamBug;

	TSubclassOf<ASteamBug> SteamBug_ClassBP;
	TSubclassOf<ASteamBug> SteamBug_Class;

	friend class UInteractiveWithPuzzlesComponent;
};
