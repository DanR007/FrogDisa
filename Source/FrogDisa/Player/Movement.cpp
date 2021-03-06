#include "Movement.h"
#include "SaveGameSystem.h"

#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"

#include "FrogDisa/MyHUD.h"
#include "FrogDisa/CarriedObject.h"
#include "FrogDisa/InteractiveObjectsInterface.h"
#include "FrogDisa/DefaultVariables.h"

#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "Blueprint/UserWidget.h"
#include "DrawDebugHelpers.h"
#include "Styling/SlateWidgetStyleContainerInterface.h"

#include<map>

#define TEST

#define ECC_ClimbingTraceChannel ECC_GameTraceChannel1
AMovement* PlayerActor;
std::map<EWeaponType, FString> weaponArrayType;

FVector offset = FVector::ZeroVector;

// Sets default values
AMovement::AMovement()
{
	//ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/Meshes/Animation/jaba.jaba'"));

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	InteractiveComponent = CreateDefaultSubobject<UInteractiveComponent>(TEXT("InteractiveComponent"));
	weaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("AttackComponent"));
	grapplingComponent = CreateDefaultSubobject<UGrapplingComponent>(TEXT("GrapplingComponent"));
	HUDComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HUD Component"));
	shopComponent = CreateDefaultSubobject<UShopComponent>(TEXT("Shop Component"));
	//AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("AttributeSet"));
	//AbilityComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));


	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	
	GetCapsuleComponent()->SetCapsuleSize(DefaultCapsuleRadius, DefaultCapsuleHalfHeight);
	//bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	//bUseControllerRotationRoll = false;
	FVector MeshPosition = FVector(0.f, 0.f, -80.f);
	FRotator MeshRotation = FRotator(0.f, 270.f, 0.f);
	
	
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation(FVector(0, 0, DefaultCameraHeight)); // Position the camera
	Camera->bUsePawnControlRotation = true;
	GetMesh()->SetupAttachment(Camera);
	//GetMesh()->SetSkeletalMesh(mesh.Object);
	//GetMesh()->SetRelativeLocationAndRotation(MeshPosition, MeshRotation);

	isGrappling = false;
	MeleeAttackIsActive = false;
	isBearObject = false;
	isRunning = false;
	pauseMenuOpen = false;
	isClimbing = false;
	isWaitingWrench = false;
	nearClimbingObject = false;
	isCrouching = false;

	weaponArrayType[EWeaponType::EW_CrossbowBolt] = "CrossbowBolt";

	PlayerActor = this;
#ifdef TEST
	
#else
	
#endif

	Collectibles = 0;
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

// Called to bind functionality to input
void AMovement::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMovement::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMovement::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AMovement::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AMovement::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Run", this, &AMovement::Run);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMovement::Jump);

	PlayerInputComponent->BindAxis("LookRight", this, &AMovement::LookRight);
	PlayerInputComponent->BindAxis("LookLeft", this, &AMovement::LookLeft);

	PlayerInputComponent->BindAxis("CallCircleMenu", this, &AMovement::CallCircleMenu);

	PlayerInputComponent->BindAxis("Attack", weaponComponent, &UWeaponComponent::Attack);
	//need to check time hold a left mouse button
	PlayerInputComponent->BindAction("UseSecondWeapon", IE_Pressed, weaponComponent, &UWeaponComponent::Fire);
	
	PlayerInputComponent->BindAction("TakeInteractiveObject", IE_Pressed, InteractiveComponent, &UInteractiveComponent::InteractionWithObject);

	PlayerInputComponent->BindAction("GrapplingHook", IE_Pressed, grapplingComponent, &UGrapplingComponent::ChangeActiveGrapplingMode);
	PlayerInputComponent->BindAction("UseSecondWeapon", IE_Pressed, grapplingComponent, &UGrapplingComponent::StartGrappling);
	
	PlayerInputComponent->BindAction("StartTakedown", IE_Pressed, this,
			&AMovement::StartStrangling);

	PlayerInputComponent->BindAction("PauseMenu", IE_Pressed, this, &AMovement::PauseMenu);

	PlayerInputComponent->BindAction("ChoiceBug", IE_Pressed, weaponComponent, &UWeaponComponent::ChoiceBug);
	PlayerInputComponent->BindAction("ChoicePistol", IE_Pressed, weaponComponent, &UWeaponComponent::ChoicePistol);
	PlayerInputComponent->BindAction("ChoiceMine", IE_Pressed, weaponComponent, &UWeaponComponent::ChoiceMine);
	PlayerInputComponent->BindAction("ChoiceCrossbowBolt", IE_Pressed, weaponComponent, &UWeaponComponent::ChoiceCrossbowBolt);
	PlayerInputComponent->BindAction("ChoiceTranquilizer", IE_Pressed, weaponComponent, &UWeaponComponent::ChoiceTranquilizer);
	PlayerInputComponent->BindAction("ChoiceGrenade", IE_Pressed, weaponComponent, &UWeaponComponent::ChoiceGrenade);

	PlayerInputComponent->BindAction("ChangeCrouchMode", IE_Pressed, this, &AMovement::ChangeCrouchMode);

	PlayerInputComponent->BindAction("Heal", IE_Pressed, HUDComponent, &UHealthComponent::Heal);
	
}


// Called when the game starts or when spawned
void AMovement::BeginPlay()
{
	queryParams.AddIgnoredActor(this);
	

	Super::BeginPlay();
	
}

// Called every frame
void AMovement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanMakeAction())
	{
		if (InteractiveComponent)
		{
			InteractiveComponent->CheckInteractiveObject();
		}
		grapplingComponent->ChoiceGrapplingVariant();
	}

	if (isCrouching)
	{
		ChangeCrouchHeight();
	}
}

void AMovement::MoveForward(float Value)
{
	////GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::SanitizeFloat(Value));
	if (Controller && Value != 0.0f)
	{
		if (isClimbing)
			SetActorLocation(GetActorLocation() + GetActorUpVector() * Value);
			//AddMovementInput(GetActorUpVector(), Value);
		else
			AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMovement::MoveRight(float Value)
{
	if (Controller && Value != 0.0f)
	{
		if (!isClimbing)
			AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMovement::LookRight(float Value)
{
	if (Value != 0)
	{
		isLookRight = true;
		if (!isLookLeft && nowOffsetY < RightOffsetY)
		{
			nowOffsetY += Value * 10;
			
			Camera->SetRelativeLocation(FVector(0, nowOffsetY, nowOffsetZ));
		}
	}
	else
	{
		isLookRight = false;
		if (!isLookLeft && nowOffsetY > DefaultOffsetY)
		{
			nowOffsetY -= 10.f;
			Camera->SetRelativeLocation(FVector(0, nowOffsetY, nowOffsetZ));
		}
	}
}

void AMovement::LookLeft(float Value)
{
	if (Value != 0)
	{
		isLookLeft = true;
		if (!isLookRight && nowOffsetY > LeftOffsetY)
		{
			nowOffsetY -= Value * 10;
			Camera->SetRelativeLocation(FVector(0, nowOffsetY, nowOffsetZ));
		}
	}
	else
	{
		isLookLeft = false;
		if (!isLookRight && nowOffsetY < DefaultOffsetY)
		{
			nowOffsetY += 10.f;
			Camera->SetRelativeLocation(FVector(0, nowOffsetY, nowOffsetZ));
		}
	}
}

void AMovement::Run(float Value)
{
	if (Value != 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
	else
	{
		isRunning = false;
		if(isStrangling)
			GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed / 4;
		else
			GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	}
}

void AMovement::CallCircleMenu(float Value)
{
	if (Value != 0)
	{
		circleMenuIsOpen = true;
	}
	else
	{
		circleMenuIsOpen = false;
	}
}

void AMovement::Attack()
{
	if (isBearObject)
	{
		isBearObject = false;
		InteractiveComponent->DropInteractiveObject(ThrowImpulseValue);
	}
}

void AMovement::SetNewPosses_Implementation(APawn* new_character)
{

}

void AMovement::SetMeleeAttackInactive()
{
	MeleeAttackIsActive = false;
}

void AMovement::ChangeCrouchMode()
{
	if (!isCrouching)
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(DefaultCapsuleHalfHeight / 2);
		Camera->SetRelativeLocation(FVector(0, 0, DefaultCameraHeight / 2));
		isCrouching = true;
		nowOffsetZ = DefaultCameraHeight / 2;
	}
	else
	{
		if (CheckCanStand() == false)
		{
			AddActorWorldOffset(FVector(0, 0, DefaultCapsuleHalfHeight / 2));
			GetCapsuleComponent()->SetCapsuleHalfHeight(DefaultCapsuleHalfHeight);
			Camera->SetRelativeLocation(FVector(0, 0, DefaultCameraHeight));
			isCrouching = false;
			nowOffsetZ = DefaultCameraHeight;
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "YOU CANT STAND NOW");
			UE_LOG(LogTemp, Warning, TEXT("YOU CANT STAND NOW"))
		}
	}
}

void AMovement::AddControllerYawInput(float Val)
{
	if (Val != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		APlayerController* const PC = CastChecked<APlayerController>(Controller);
		PC->AddYawInput(Val);
	}
}

void AMovement::AddControllerPitchInput(float Val)
{
	if (Val != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		APlayerController* const PC = CastChecked<APlayerController>(Controller);
		PC->AddPitchInput(Val);
	}
}

void AMovement::Jump()
{
	if (GetCharacterMovement()->IsFalling() == false 
		|| GetCharacterMovement()->IsFlying() || GetCharacterMovement()->MovementMode == EMovementMode::MOVE_None)
	{
		if (GetCharacterMovement()->IsFlying())
		{
			grapplingComponent->StopGrappling();
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
		else
		{
			if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_None)
			{
				GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			}
		}
		FHitResult upTrace, forwardTrace;
		GetWorld()->LineTraceSingleByChannel(forwardTrace, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 80, GRAPPLING_TRACE_CHANNEL, queryParams);
		
		FVector upStart = GetActorLocation() + GetActorForwardVector() * (DefaultCapsuleRadius + 10) + GetActorUpVector() * GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 3,
			upEnd = GetActorLocation() + GetActorForwardVector() * (DefaultCapsuleRadius + 10);

		if (GetWorld()->LineTraceSingleByChannel(upTrace, upStart, upEnd, GRAPPLING_TRACE_CHANNEL, queryParams))
		{
			if (FVector::Distance(upEnd, upTrace.Location) < GetCapsuleComponent()->GetScaledCapsuleHalfHeight())
			{
				GetCharacterMovement()->SetMovementMode(MOVE_Flying);
				GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				endLoc = upTrace.Location + FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 2);
				GetWorldTimerManager().SetTimer(ClimbTimer, this, &AMovement::Climbing, 0.05f, true);
			}
			else
			{
				bPressedJump = true;
				JumpKeyHoldTime = 0.0f;
			}
		}
		else
		{
			bPressedJump = true;
			JumpKeyHoldTime = 0.0f;
		}
	}
}

void AMovement::StopJumping()
{
	bPressedJump = false;
	ResetJumpState();
}


void AMovement::PauseMenu()
{
	pauseMenuOpen = true;
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	GetWorld()->GetFirstPlayerController()->SetPause(true);
}

void AMovement::Climbing()
{
	if (FVector::Distance(GetActorLocation(), endLoc) < 10)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMovementComponent()->Velocity = FVector::ZeroVector;
		GetWorldTimerManager().ClearTimer(ClimbTimer);
	}
	else
	{
		GetMovementComponent()->Velocity = (endLoc - GetActorLocation()) * 3;
	}
}

void AMovement::ChangeCrouchHeight()
{
	FHitResult hit_res_forward, hit_res_up;
	FCollisionShape box;
	TArray<AActor*> arr_ignored_actors;

	//GetAllChildActors(arr_ignored_actors);
	//queryParams.AddIgnoredActors(arr_ignored_actors);

	box.SetBox(FVector(DefaultCapsuleRadius, DefaultCapsuleRadius, DefaultCapsuleHalfHeight / 2));
	FVector StartForwardToUp = GetActorLocation() + GetActorForwardVector() * DefaultCapsuleRadius + offset,
		EndForwardToUp = GetActorLocation() + FVector(DefaultCapsuleRadius, 0, DefaultCapsuleHalfHeight / 2) + offset,
		StartHereToUp = GetActorLocation() + offset, EndHereToUp = GetActorLocation() + GetActorUpVector() * DefaultCapsuleHalfHeight / 2 + offset;

	

	if (GetWorld()->SweepSingleByChannel(hit_res_forward, StartForwardToUp, EndForwardToUp
		, GetActorRotation().Quaternion(), ECollisionChannel::ECC_Visibility, box, queryParams) ||
		GetWorld()->SweepSingleByChannel(hit_res_up, StartHereToUp, EndHereToUp
			, GetActorRotation().Quaternion(), ECollisionChannel::ECC_Visibility, box, queryParams))
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(DefaultCapsuleHalfHeight / 3);
		Camera->SetRelativeLocation(FVector(0, nowOffsetY, DefaultCameraHeight / 3));
		offset = FVector(0, 0, DefaultCapsuleHalfHeight / 3);
		
	}
	else
	{
		if(offset != FVector::ZeroVector)
			SetActorLocation(GetActorLocation() + FVector(0, 0, DefaultCapsuleHalfHeight / 2 - DefaultCapsuleHalfHeight / 3));
		GetCapsuleComponent()->SetCapsuleHalfHeight(DefaultCapsuleHalfHeight / 2);
		Camera->SetRelativeLocation(FVector(0, nowOffsetY, DefaultCameraHeight / 2));
		offset = FVector::ZeroVector;
	}
	if(hit_res_up.GetActor())
	GEngine->AddOnScreenDebugMessage(-1, 0.1, FColor::Red, hit_res_up.GetActor()->GetName());
}

bool AMovement::CheckCanStand()
{
	FHitResult hit_res;
	FCollisionShape box;
	TArray<AActor*> arr_ignored_actors;
	GetAllChildActors(arr_ignored_actors);
	queryParams.AddIgnoredActors(arr_ignored_actors);
	box.SetBox(FVector(DefaultCapsuleRadius, DefaultCapsuleRadius, DefaultCapsuleHalfHeight / 2));

	FVector StartUpper = GetActorLocation(), EndUpper = GetActorLocation() + GetActorUpVector() * GetCapsuleComponent()->GetScaledCapsuleHalfHeight()/*DefaultCapsuleHalfHeight / 3*/;

	//if (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() == DefaultCapsuleHalfHeight / 2)
	//	EndUpper = GetActorLocation() + GetActorUpVector() * DefaultCapsuleHalfHeight / 2;

	return GetWorld()->SweepSingleByChannel(hit_res, StartUpper, EndUpper
		, GetActorRotation().Quaternion(), ECollisionChannel::ECC_Visibility, box, queryParams);
}

void AMovement::DrawGrapplingVariant_Implementation()
{

}


bool AMovement::CanMakeAction() const
{
	return !isBearObject;
}


bool AMovement::IsBearObject() const
{
	return isBearObject;
}

int AMovement::GetCountCollectibles() const
{
	return Collectibles;
}

void AMovement::SetStartSettings(int countCollectibles, bool isHaveBug, bool isBearObj)
{
	Collectibles = countCollectibles;
	isBearObject = isBearObj;
}


bool AMovement::GetPauseState() const
{
	return pauseMenuOpen;
}

void AMovement::SetUnPause()
{
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	pauseMenuOpen = false;
}

//UAbilitySystemComponent* AMovement::GetAbilitySystemComponent() const
//{
//	return AbilityComponent;
//}

void AMovement::StartStranglingBP_Implementation()
{

}

void AMovement::SetIsClimbing(bool is_climbing)
{
	isClimbing = is_climbing;
	if (isClimbing)
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	else
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	//GetCapsuleComponent()->SetEnableGravity(!isClimbing);
}