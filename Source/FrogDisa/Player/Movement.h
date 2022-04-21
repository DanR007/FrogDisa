#pragma once

#include "CoreMinimal.h"

#include "EProjectiles.h"
#include "SteamBug.h"
#include "ShootComponent.h"
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

	bool CanMakeAction();

	void ChangeCharacter();

	void PauseMenu();

	UFUNCTION(BlueprintCallable)
		void SetMeleeAttackInactive();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool isClimbing;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetNewPosses();

	

	void ChoiceWrench() 
	{ 
		if (g_Projectile_Type != EWeaponType::EW_Wrench)
		{
			g_Projectile_Type = EWeaponType::EW_Wrench; GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, "Wrench");
			if (shootComponent)
				shootComponent->SwitchProjectile();
		}
	}
	void ChoicePistol()  
	{ 
		if (g_Projectile_Type != EWeaponType::EW_Pistol)
		{
			g_Projectile_Type = EWeaponType::EW_Pistol; GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, "Pistol");
			if (shootComponent)
				shootComponent->SwitchProjectile();
		}
	}
	void ChoiceMine()   
	{
		if (g_Projectile_Type != EWeaponType::EW_Mine)
		{
			g_Projectile_Type = EWeaponType::EW_Mine; GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, "Mine");
			if (shootComponent)
				shootComponent->SwitchProjectile();
		}
	}
	void ChoiceCrossbowBolt()
	{
		if (g_Projectile_Type != EWeaponType::EW_CrossbowBolt)
		{
			g_Projectile_Type = EWeaponType::EW_CrossbowBolt; GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, "CrossbowBolt");
			if (shootComponent)
				shootComponent->SwitchProjectile();
		}
	}
	void ChoiceTranquilizer()
	{
		if (g_Projectile_Type != EWeaponType::EW_Tranquilizer)
		{
			g_Projectile_Type = EWeaponType::EW_Tranquilizer; GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow , "Tranquilizer");
			if (shootComponent)
				shootComponent->SwitchProjectile();
		}
	}
	void ChoiceGrenade()
	{
		if (g_Projectile_Type != EWeaponType::EW_Grenade)
		{
			g_Projectile_Type = EWeaponType::EW_Grenade; GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, "granataaaaaaaaaaaaa");
			if (shootComponent)
				shootComponent->SwitchProjectile();
		}
	}

	UFUNCTION(BlueprintCallable)
		UGrapplingComponent* GetGrapplingComponent() { return grapplingComponent; }

	void ChangeCrouchMode();

	void LookRight(float Value);
	void LookLeft(float Value);

	void ChangeCrouchHeight();
public:
	UFUNCTION(BlueprintCallable)
		bool GetCrouchMode() { return isCrouching; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "meshComponent")
		UStaticMeshComponent* meshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
		UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShootComp")
		UShootComponent* shootComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive")
		UInteractiveComponent* InteractiveComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UFrogDisaGameInstance* game_Instance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UGrapplingComponent* grapplingComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UHealthComponent* HUDComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UShopComponent* shopComponent;
	//UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	//	UAbilitySystemComponent* AbilityComponent;
	//UPROPERTY(Transient)
	//	UCharacterAttributeSet* AttributeSet;


	UFUNCTION(BlueprintCallable)
		void SetStartSettings(int countCollectibles, bool isHaveBug, bool isBearObj);
	
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
		int GetCountCollectibles();
	UFUNCTION(BlueprintCallable)
	EWeaponType GetCurrentWeaponType();

	UFUNCTION(BlueprintCallable)
		bool GetIsGrappling(){ return isGrappling; }

	UFUNCTION(BlueprintCallable)
		bool GetIsStrangling() { return isStrangling; }

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
	//UPawnMovementComponent* GetMovementForComponents() { return GetMovementComponent(); }

	void StartStrangling() {
		if (canStrangling) {
			isStrangling = true;
	} }

	FTimeline MyTimeline;
	float CurveFloatValue;
	float TimelineValue;
	FTimerHandle GrapplingTimer;

	bool isBearObject;
	bool isRunning;
	bool nearClimbingObject;
	bool isGrappling;
	bool circleMenuIsOpen;
	bool pauseMenuOpen;
	bool isWaitingWrench;
	bool HaveSteamBug;
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

	const float DefaultCapsuleHeight = 135;
	const float DefaultCameraHeight = 130.5;

	const float DefaultOffsetY = 0;
	const float LeftOffsetY = -60;
	const float RightOffsetY = 60;
	float nowOffsetY = 0;
	float nowOffsetZ = DefaultCameraHeight;
	float CapsuleRadius;

	EWeaponType g_Projectile_Type;

	FCollisionQueryParams CollisionParams;
	AMovableObject* movableActor;
	ASteamBug* SteamBug;
	FCollisionQueryParams queryParams;

	TSubclassOf<ASteamBug> SteamBug_ClassBP;
	TSubclassOf<ASteamBug> SteamBug_Class;

	friend class UInteractiveWithPuzzlesComponent;
	//friend class ACollectiblesObject;
	friend class UGrapplingComponent;
	friend class UInteractiveComponent;
};
