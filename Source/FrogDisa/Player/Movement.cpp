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

	ConstructorHelpers::FClassFinder<ASteamBug> steam_bug_bp(TEXT("/Game/Blueprint/BP_SteamBug"));
	//ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/Meshes/Animation/jaba.jaba'"));

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	InteractiveComponent = CreateDefaultSubobject<UInteractiveComponent>(TEXT("InteractiveComponent"));
	shootComponent = CreateDefaultSubobject<UShootComponent>(TEXT("ShootComponent"));
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


	SteamBug_ClassBP = steam_bug_bp.Class;

	isGrappling = false;
	MeleeAttackIsActive = false;
	isBearObject = false;
	isRunning = false;
	pauseMenuOpen = false;
	isClimbing = false;
	isWaitingWrench = false;
	nearClimbingObject = false;
	isCrouching = false;

	weaponArrayType[EWeaponType::EW_Wrench] = "Wrench";
	weaponArrayType[EWeaponType::EW_CrossbowBolt] = "CrossbowBolt";

	PlayerActor = this;
#ifdef TEST
	HaveSteamBug = true;
#else
	HaveSteamBug = false;
#endif

	Collectibles = 0;

	g_Projectile_Type = EWeaponType::EW_Wrench;
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

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMovement::Attack);
	PlayerInputComponent->BindAction("UseSecondWeapon", IE_Pressed, shootComponent, &UShootComponent::Fire);
	
	PlayerInputComponent->BindAction("TakeInteractiveObject", IE_Pressed, InteractiveComponent, &UInteractiveComponent::InteractionWithObject);

	PlayerInputComponent->BindAction("GrapplingHook", IE_Pressed, grapplingComponent, &UGrapplingComponent::ChangeActiveGrapplingMode);
	PlayerInputComponent->BindAction("UseSecondWeapon", IE_Pressed, grapplingComponent, &UGrapplingComponent::StartGrappling);
	
	PlayerInputComponent->BindAction("StartTakedown", IE_Pressed, this,
			&AMovement::StartStrangling);

	PlayerInputComponent->BindAction("PauseMenu", IE_Pressed, this, &AMovement::PauseMenu);

	PlayerInputComponent->BindAction("SwitchCharacter", IE_Pressed, this, &AMovement::ChangeCharacter);

	PlayerInputComponent->BindAction("ChoiceWrench", IE_Pressed, this, &AMovement::ChoiceWrench);
	PlayerInputComponent->BindAction("ChoicePistol", IE_Pressed, this, &AMovement::ChoicePistol);
	PlayerInputComponent->BindAction("ChoiceMine", IE_Pressed, this, &AMovement::ChoiceMine);
	PlayerInputComponent->BindAction("ChoiceCrossbowBolt", IE_Pressed, this, &AMovement::ChoiceCrossbowBolt);
	PlayerInputComponent->BindAction("ChoiceTranquilizer", IE_Pressed, this, &AMovement::ChoiceTranquilizer);
	PlayerInputComponent->BindAction("ChoiceGrenade", IE_Pressed, this, &AMovement::ChoiceGrenade);

	PlayerInputComponent->BindAction("ChangeCrouchMode", IE_Pressed, this, &AMovement::ChangeCrouchMode);
	
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
		ForwardTrace();
		HeightTrace();
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
		AddMovementInput(GetActorForwardVector(), Value);

	}
}

void AMovement::MoveRight(float Value)
{
if (Controller && Value != 0.0f)
	{
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

void AMovement::ChangeCharacter()
{
	if (HaveSteamBug && GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FTransform bugTransform = FTransform(GetActorRotation(), GetMesh()->GetSocketLocation("feet_L") + FVector(70,0,0));
		SteamBug = GetWorld()->SpawnActor<ASteamBug>(SteamBug_ClassBP, bugTransform, SpawnParameters);
		SteamBug->SetMainCharacter(this);
		SteamBug->SetNewPosses();
	}
}

void AMovement::SetNewPosses_Implementation()
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
		|| GetCharacterMovement()->IsFlying())
	{
		if (GetCharacterMovement()->IsFlying())
		{
			grapplingComponent->StopGrappling();
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
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
				//while (FVector::Distance(GetActorLocation(), jumpLoc) > 5)
				//{
					//GEngine->AddOnScreenDebugMessage(-1, 0.1, FColor::Red, "climb");
				//	GetMovementComponent()->Velocity = (jumpLoc - GetActorLocation()) / 2;
				//}
				//GetCharacterMovement()->SetMovementMode(MOVE_Falling);
				//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				//SetActorLocation(upTrace.Location + FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));

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
		//make here climbing on objects
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


void AMovement::ForwardTrace()
{
	FHitResult hitPoint;
	FVector f_start, f_end;
	f_start = GetActorLocation();
	f_end = GetActorLocation() + GetActorForwardVector() * 100;

	if(GetWorld()->LineTraceSingleByChannel(hitPoint, f_start, f_end, ECC_ClimbingTraceChannel, queryParams))//check an object height
	{
		wallNormal = hitPoint.Normal;
		wallLocation = hitPoint.Location;
	}
}

void AMovement::HeightTrace()
{
	FHitResult hitPoint;
	FVector h_start, h_end;
	h_start = GetActorLocation() + FVector(0, 0, 200.f) + GetActorForwardVector() * 75;
	h_end = h_start - FVector(0, 0, 200.f);

	if (GetWorld()->LineTraceSingleByChannel(hitPoint, h_start, h_end, ECC_ClimbingTraceChannel, queryParams))//check an object in front of player
	{
#ifdef THIRD_PERSON
		float distanceZ = Mesh->GetSocketLocation(TEXT("spineSocket")).Z - hitPoint.Location.Z;
		if (distanceZ <= 0 && distanceZ >= -100.f)
		{
			if (!isClimbing && GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling)
			{

				isClimbing = true;
				GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
				GetCharacterMovement()->StopMovementImmediately();
			}
		}
#else
		float distanceZ = Camera->GetComponentLocation().Z - hitPoint.Location.Z;
		if (distanceZ <= 0 && distanceZ >= -100.f)
		{
			nearClimbingObject = true;
		}
#endif

		
	}
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
	FCollisionShape capsule;
	TArray<AActor*> arr_ignored_actors;

	//GetAllChildActors(arr_ignored_actors);
	//queryParams.AddIgnoredActors(arr_ignored_actors);

	capsule.SetBox(FVector(DefaultCapsuleRadius, DefaultCapsuleRadius, DefaultCapsuleHalfHeight / 2));
	FVector StartForwardToUp = GetActorLocation() + GetActorForwardVector() * DefaultCapsuleRadius + offset,
		EndForwardToUp = GetActorLocation() + FVector(DefaultCapsuleRadius, 0, DefaultCapsuleHalfHeight / 2) + offset,
		StartHereToUp = GetActorLocation() + offset, EndHereToUp = GetActorLocation() + GetActorUpVector() * DefaultCapsuleHalfHeight / 2 + offset;

	

	if (GetWorld()->SweepSingleByChannel(hit_res_forward, StartForwardToUp, EndForwardToUp
		, GetActorRotation().Quaternion(), ECollisionChannel::ECC_Visibility, capsule, queryParams) ||
		GetWorld()->SweepSingleByChannel(hit_res_up, StartHereToUp, EndHereToUp
			, GetActorRotation().Quaternion(), ECollisionChannel::ECC_Visibility, capsule, queryParams))
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
	FCollisionShape capsule;
	TArray<AActor*> arr_ignored_actors;
	GetAllChildActors(arr_ignored_actors);
	queryParams.AddIgnoredActors(arr_ignored_actors);
	capsule.SetCapsule(DefaultCapsuleRadius, DefaultCapsuleHalfHeight);

	FVector StartUpper = GetActorLocation(), EndUpper = GetActorLocation() + GetActorUpVector() * GetCapsuleComponent()->GetScaledCapsuleHalfHeight()/*DefaultCapsuleHalfHeight / 3*/;

	//if (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() == DefaultCapsuleHalfHeight / 2)
	//	EndUpper = GetActorLocation() + GetActorUpVector() * DefaultCapsuleHalfHeight / 2;

	return GetWorld()->SweepSingleByChannel(hit_res, StartUpper, EndUpper
		, GetActorRotation().Quaternion(), ECollisionChannel::ECC_Visibility, capsule, queryParams);
}

void AMovement::DrawGrapplingVariant_Implementation()
{

}


bool AMovement::CanMakeAction() const
{
	return !isBearObject;
}


bool AMovement::IsHaveASteamBug() const
{
	return HaveSteamBug;
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
	HaveSteamBug = isHaveBug;
	isBearObject = isBearObj;
}

EWeaponType AMovement::GetCurrentWeaponType() const
{
	return g_Projectile_Type;
}

AActor* AMovement::GetThrowProjectile() const
{
	return shootComponent->GetActorWrench();
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