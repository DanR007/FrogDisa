#pragma once

#include "CoreMinimal.h"

#include "EProjectiles.h"
#include "WeaponComponent.h"
#include "InteractiveComponent.h"
#include "FrogDisaGameInstance.h"
#include "GrapplingComponent.h"
#include "HealthComponent.h"
#include "ShopComponent.h"

#include "AbilitySystemInterface.h"

#include "FrogDisa/MovableObject.h"
#include "FrogDisa/CharacterAttributeSet.h"

#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/HUD.h"

#include "Components/TimelineComponent.h"
#include "Components/BoxComponent.h"

#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "Movement.generated.h"

//#define THIRD_PERSON


UCLASS()
class FROGDISA_API AMovement : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMovement();
	
	//UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool MeleeAttackIsActive;

		void AddCollectibles() { Collectibles++; }
		
		
		UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
			void DrawGrapplingVariant();

		UFUNCTION(BlueprintCallable)
			void SetCanStrangling() { canStrangling = true; }
		UFUNCTION(BlueprintCallable)
			void SetCantStrangling() { canStrangling = false; }

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
			ACharacter* StrangledCharacter;

		UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
			void SetNewPosses(APawn* new_character);

		void SetIsClimbing(bool is_climbing);
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

	void Run(float Value);

	bool CanMakeAction() const;

	void PauseMenu();

	UFUNCTION(BlueprintCallable)
		void SetMeleeAttackInactive();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool isClimbing;

	

	

	UFUNCTION(BlueprintCallable)
		UGrapplingComponent* GetGrapplingComponent() const { return grapplingComponent; }

	void ChangeCrouchMode();

	void LookRight(float Value);
	void LookLeft(float Value);

	void ChangeCrouchHeight();
public:
	UFUNCTION(BlueprintCallable)
		bool GetCrouchMode() const { return isCrouching; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components/MainComponents")
		UStaticMeshComponent* meshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components/MainComponents")
		UCameraComponent* Camera;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components/AdditionalComponents")
		UWeaponComponent* weaponComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components/AdditionalComponents")
		UInteractiveComponent* InteractiveComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components/AdditionalComponents")
		UFrogDisaGameInstance* game_Instance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components/AdditionalComponents")
		UGrapplingComponent* grapplingComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components/AdditionalComponents")
		UHealthComponent* HUDComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components/AdditionalComponents")
		UShopComponent* shopComponent;
	//UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	//	UAbilitySystemComponent* AbilityComponent;
	//UPROPERTY(Transient)
	//	UCharacterAttributeSet* AttributeSet;


	UFUNCTION(BlueprintCallable)
		void SetStartSettings(int countCollectibles, bool isHaveBug, bool isBearObj);

	UFUNCTION(BlueprintCallable)
		bool GetPauseState() const;
	UFUNCTION(BlueprintCallable)
		void SetUnPause();

	UFUNCTION(BlueprintCallable)
		bool IsBearObject() const;


	UFUNCTION(BlueprintCallable)
		int GetCountCollectibles() const;

	UFUNCTION(BlueprintCallable)
		bool GetIsGrappling() const { return isGrappling; }

	UFUNCTION(BlueprintCallable)
		bool GetIsStrangling() const { return isStrangling; }

	UFUNCTION(BlueprintCallable)
		void SetStartStrangling() { isStrangling = true; }

	UFUNCTION(BlueprintNativeEvent)
		void StartStranglingBP();

	bool CheckCanStand();

	void CallCircleMenu(float Value);
private:
	void ForwardTrace();
	void HeightTrace();
	void Fire();

	void Climbing();
	//UPawnMovementComponent* GetMovementForComponents() { return GetMovementComponent(); }

	void StartStrangling() {
		if (canStrangling) {
			isStrangling = true;
	} }

	FTimeline MyTimeline;
	float CurveFloatValue;
	float TimelineValue;
	FTimerHandle ClimbTimer;

	bool isBearObject;
	bool isRunning;
	bool nearClimbingObject;
	bool isGrappling;
	bool circleMenuIsOpen;
	bool pauseMenuOpen;
	bool isWaitingWrench;
	bool isCrouching;
	bool isStrangling = false;
	bool canStrangling = false;
	bool isLookLeft;
	bool isLookRight;


	int Collectibles;
	int stoneCount;

	FVector endLoc;
	FVector wallNormal, wallLocation;

	const float DefaultSpeed = 400.f, RunningSpeed = DefaultSpeed + 200.f;
	const float ChangeTargetArmSpeed = 20.f;
	const float ThrowImpulseValue = 1500.f;
	const float DropImpulseValue = 0;

	const float DefaultCapsuleHalfHeight = 90;
	const float DefaultCapsuleRadius = 20;
	const float DefaultCameraHeight = 80;

	const float DefaultOffsetY = 0;
	const float LeftOffsetY = -60;
	const float RightOffsetY = 60;
	float nowOffsetY = 0;
	float nowOffsetZ = DefaultCameraHeight;

	FCollisionQueryParams CollisionParams;
	AMovableObject* movableActor;
	FCollisionQueryParams queryParams;


	friend class UInteractiveWithPuzzlesComponent;
	friend class UGrapplingComponent;
	friend class UInteractiveComponent;
	friend class UWeaponComponent;
};
