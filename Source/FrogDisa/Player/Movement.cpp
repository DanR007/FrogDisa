#include "Movement.h"
#include "SaveGameSystem.h"

#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

#include "FrogDisa/MyHUD.h"
#include "FrogDisa/CarriedObject.h"
#include "FrogDisa/InteractiveObjectsInterface.h"

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

std::map<EWeaponType, FString> weaponArrayType;

// Sets default values
AMovement::AMovement()
{

	ConstructorHelpers::FClassFinder<ASteamBug> steam_bug_bp(TEXT("/Game/Blueprint/BP_SteamBug"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/Meshes/Animation/jaba.jaba'"));

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	InteractiveComponent = CreateDefaultSubobject<UInteractiveComponent>(TEXT("InteractiveComponent"));
	InteractiveWithPuzzlesComponent = CreateDefaultSubobject<UInteractiveWithPuzzlesComponent>(TEXT("InteractiveWithPuzzlesComponent"));
	shootComponent = CreateDefaultSubobject<UShootComponent>(TEXT("ShootComponent"));
	grapplingComponent = CreateDefaultSubobject<UGrapplingComponent>(TEXT("GrapplingComponent"));
	//AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("AttributeSet"));
	//AbilityComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));

	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	GetCapsuleComponent()->SetCapsuleSize(20.f, DefaultCapsuleHeight);
#ifdef THIRD_PERSON
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	cameraComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("springarmcamera"));
	cameraComponent->bUsePawnControlRotation = true;
	cameraComponent->TargetArmLength = 400.f;
	cameraComponent->SetupAttachment(RootComponent);
	Camera->SetupAttachment(cameraComponent, USpringArmComponent::SocketName);;
	Camera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	FVector MeshPosition = FVector(0.f, 0.f, -80.f);
	FRotator MeshRotation = FRotator(0.f, 270.f, 0.f);
	GetMesh()->SetSkeletalMesh(mesh.Object);
	GetMesh()->SetRelativeLocationAndRotation(MeshPosition, MeshRotation);

	
#else
	//bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	//bUseControllerRotationRoll = false;
	FVector MeshPosition = FVector(0.f, 0.f, -80.f);
	FRotator MeshRotation = FRotator(0.f, 270.f, 0.f);
	
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation(FVector(0, 1.75f, DefaultCameraHeight)); // Position the camera
	Camera->bUsePawnControlRotation = true;
	GetMesh()->SetupAttachment(Camera);
	//GetMesh()->SetSkeletalMesh(mesh.Object);
	//GetMesh()->SetRelativeLocationAndRotation(MeshPosition, MeshRotation);
#endif

	SteamBug_ClassBP = steam_bug_bp.Class;

	isGrappling = false;
	MeleeAttackIsActive = false;
	isBearObject = false;
	isAiming = false;
	isRunning = false;
	pauseMenuOpen = false;
	isClimbing = false;
	isWaitingWrench = false;
	nearClimbingObject = false;
	isCrouching = false;

	weaponArrayType[EWeaponType::EW_Wrench] = "Wrench";

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
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMovement::StopJumping);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMovement::Attack);
	PlayerInputComponent->BindAction("UseSecondWeapon", IE_Pressed, this, &AMovement::Fire);
	
	PlayerInputComponent->BindAction("TakeInteractiveObject", IE_Pressed, this, &AMovement::InteractionWithObject);

	PlayerInputComponent->BindAction("GrapplingHook", IE_Pressed, grapplingComponent, &UGrapplingComponent::ChangeActiveGrapplingMode);
	PlayerInputComponent->BindAction("UseSecondWeapon", IE_Pressed, grapplingComponent, &UGrapplingComponent::StartGrappling);

	//PlayerInputComponent->BindAction("ActionWithSomeObj", IE_Pressed, this, &AMovement::InteractObject);
	PlayerInputComponent->BindAction("ActionWithSomeObj", IE_Pressed, InteractiveWithPuzzlesComponent,
		&UInteractiveWithPuzzlesComponent::ActionWithPuzzleActor);

	PlayerInputComponent->BindAction("PauseMenu", IE_Pressed, this, &AMovement::PauseMenu);

	PlayerInputComponent->BindAction("SwitchProjectileType", IE_Pressed, this, &AMovement::SwitchProjectile);

	PlayerInputComponent->BindAction("SwitchCharacter", IE_Pressed, this, &AMovement::ChangeCharacter);

	PlayerInputComponent->BindAction("ChoiceWrench", IE_Pressed, this, &AMovement::ChoiceWrench);
	PlayerInputComponent->BindAction("ChoiceStone", IE_Pressed, this, &AMovement::ChoiceStone);
	PlayerInputComponent->BindAction("ChoiceMine", IE_Pressed, this, &AMovement::ChoiceMine);

	PlayerInputComponent->BindAction("ChangeCrouchMode", IE_Pressed, this, &AMovement::ChangeCrouchMode);
	
}


// Called when the game starts or when spawned
void AMovement::BeginPlay()
{
	queryParams.AddIgnoredActor(this);
	Super::BeginPlay();
	if(shootComponent)
		UE_LOG(LogTemp, Warning, TEXT("Some warning message"))
	else
		UE_LOG(LogTemp, Warning, TEXT("ShootComponent is null"))
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

		FHitResult hitPoint;

		FVector Start = GetActorLocation();
		FVector End = Start + GetActorForwardVector() * 40.f;

		if (GetWorld()->LineTraceSingleByChannel(hitPoint, Start, End, ECC_Visibility, queryParams))
		{
			movableActor = Cast<AMovableObject>(hitPoint.Actor.Get());
			if (movableActor)
			{
				movableActor->SetMovable();
			}
		}
		else
		{
			if (movableActor)
			{
				movableActor->SetStatic();
				movableActor = nullptr;
			}
		}
	}
}

void AMovement::Fire()
{
	if(shootComponent && grapplingComponent->GetGrapplingModeActive() == false)
		shootComponent->Fire();
}

void AMovement::MoveForward(float Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::SanitizeFloat(Value));
	if (Controller && Value != 0.0f)
	{
#ifdef THIRD_PERSON
		if (isAiming)
		{
			AddMovementInput(GetActorForwardVector(), Value);
		}
		else
		{
			if (isClimbing)
			{
				if (Value > 0)
				{
					GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
					ACharacter::Jump();
					isClimbing = false;
				}
				else
				{
					GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
					const FRotator Rotation = Controller->GetControlRotation();
					const FRotator YawRotation(0, Rotation.Yaw, 0);
					const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
					AddMovementInput(Direction, Value);
					isClimbing = false;
				}
			}
			else
			{
				const FRotator Rotation = Controller->GetControlRotation();
				const FRotator YawRotation(0, Rotation.Yaw, 0);
				const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
				AddMovementInput(Direction, Value);
			}
			

		}

#else
		AddMovementInput(GetActorForwardVector(), Value);
#endif
	}
}

void AMovement::MoveRight(float Value)
{
#ifdef THIRD_PERSON
	if (Controller && Value != 0.0f && isClimbing == false)
	{
		if (isAiming)
		{
			AddMovementInput(GetActorRightVector(), Value);
		}
		else
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, Value);
		}
	}
#else
	if (Controller && Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
#endif

	
 // 

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
		GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	}
}
void AMovement::Attack()
{
	if (isBearObject == false)
	{
#ifdef THIRD_PERSON
		if (isAiming == true)
		{
#ifdef TEST
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Shoot"));
#endif //TEST
			shootComponent->ThrowProjectile(g_Projectile_Type);
		}
		else
		{
			MeleeAttackIsActive = true;
			GetWorld()->GetTimerManager().SetTimer(GrapplingTimer, this, &AMovement::SetMeleeAttackInactive, 0.01f, false, 0.5f);
		}
#else
		//����� �������
		//ShootComponent->Fire();
		//ShootComponent->ThrowProjectile(g_Projectile_Type);
#endif// THIRD_PERSON
	}
	else
	{
		if (InteractiveComponent->OverlapOnlyInteractivePuzzle())
		{
			isBearObject = false;
			InteractiveComponent->DropInteractiveObject(ThrowImpulseValue);
		}
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

void AMovement::UseGrapplingHook()
{
	if (grapplingComponent->GetCanGrappling()) 
	{
		isGrappling = true;
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		GetWorld()->GetTimerManager().SetTimer(GrapplingTimer, this, &AMovement::LerpTo, 0.01f, true, 0.f);
	}
}

void AMovement::LerpTo()
{
//#ifdef THIRD_PERSON
	//UE_LOG(LogTemp, Warning, TEXT(FString::SanitizeFloat(GetCapsuleComponent()->GetScaledCapsuleRadius())))
	if (FVector::Distance(GetActorLocation(), grapplingComponent->grappling_target_location) <= GetCapsuleComponent()->GetScaledCapsuleRadius() + 5.f)//lerp while distance > 70
	{
		isGrappling = false;
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		UE_LOG(LogTemp, Warning, TEXT("Stop Grappling"))
		GetWorldTimerManager().ClearTimer(GrapplingTimer);
		GetMovementComponent()->Velocity = FVector::ZeroVector;
	}
	else
	{
		GetMovementComponent()->Velocity = (grapplingComponent->grappling_target_location - GetActorLocation()) * 10;
	}
//#else

	//GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	//float distanceToGrapplingPoint = GetDistanceTo(CheckCollectibleActors->ActorGrapplingPoint);
	//GetMovementComponent()->Velocity = (CheckCollectibleActors->ActorGrapplingPoint->GetActorLocation() - GetActorLocation())
	//	;
	
//#endif
}

void AMovement::ChangeCrouchMode()
{
	isCrouching = !isCrouching;
	SetCrouchModeSettings();
}

void AMovement::SetCrouchModeSettings()
{
	if (isCrouching)
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(DefaultCapsuleHeight / 2);
		Camera->SetRelativeLocation(FVector(0, 1.75f, DefaultCameraHeight / 2));
	}
	else
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(DefaultCapsuleHeight);
		Camera->SetRelativeLocation(FVector(0, 1.75f, DefaultCameraHeight));
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
	
#ifdef THIRD_PERSON
		if (nearClimbingObject)
		{
			//make here climbing on objects
		}
		else
		{
			bPressedJump = true;
			JumpKeyHoldTime = 0.0f;
		}
#else

		if (GetCharacterMovement()->IsFalling() == false)
		{
			bPressedJump = true;
			JumpKeyHoldTime = 0.0f;
		}
#endif
	
}

void AMovement::StopJumping()
{
	bPressedJump = false;
	ResetJumpState();
}
#ifdef THIRD_PERSON
void AMovement::Aim(float Value)
{
	if (Value != 0)
	{
		isAiming = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		Camera->bUsePawnControlRotation = true;

		cameraComponent->bUsePawnControlRotation = true;

		bUseControllerRotationYaw = true;

		if (cameraComponent->TargetArmLength > 140.f)
			cameraComponent->TargetArmLength -= ChangeTargetArmSpeed;

	}
	else
	{
		isAiming = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		Camera->bUsePawnControlRotation = false;

		cameraComponent->bUsePawnControlRotation = true;

		bUseControllerRotationYaw = false;

		if (cameraComponent->TargetArmLength < 400.f)
		{
			cameraComponent->TargetArmLength += ChangeTargetArmSpeed;
		}
	}
}
#endif

void AMovement::InteractionWithObject()
{
	IInteractiveObjectsInterface* taken_object = Cast<IInteractiveObjectsInterface>(InteractiveComponent->GetInteractiveActor());
	ICarriedObjectLogicInterface* carried_object = Cast<ICarriedObjectLogicInterface>(InteractiveComponent->GetInteractiveActor());

	if (isBearObject == true)
	{
		if (InteractiveComponent->OverlapOnlyInteractivePuzzle())
		{
			isBearObject = false;
			InteractiveComponent->DropInteractiveObject(DropImpulseValue);
		}
	}
	else
	{
		if (CanMakeAction())
		{
			if (carried_object)
			{
				isBearObject = InteractiveComponent->TakeInteractiveObject();
			}
			else
			{
				if (taken_object)
				{
					taken_object->Interact(this);
					//InteractiveComponent->InteractiveActor = nullptr;
#ifdef TEST
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Collectibles: " + FString::FromInt(Collectibles));
#endif//TEST
				}
			}
		}
	}

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

void AMovement::DrawGrapplingVariant_Implementation()
{

}

void AMovement::SwitchProjectile()
{
	g_Projectile_Type == EWeaponType::EW_Stone ? g_Projectile_Type = EWeaponType::EW_Wrench : g_Projectile_Type = EWeaponType::EW_Stone;
}

bool AMovement::CanMakeAction()
{
	return !isBearObject;
}


void AMovement::DetachInteractiveObject()
{
	isBearObject = false;
	InteractiveComponent->DetachInteractiveFromParent();
}

bool AMovement::IsHaveASteamBug()
{
	return HaveSteamBug;
}

bool AMovement::IsBearObject()
{
	return isBearObject;
}


int AMovement::GetCountStones()
{
	return stoneCount;
}

int AMovement::GetCountCollectibles()
{
	return Collectibles;
}

void AMovement::SetStartSettings(int countStone, int countCollectibles, bool isHaveBug, bool isBearObj)
{
	stoneCount = countStone;
	Collectibles = countCollectibles;
	HaveSteamBug = isHaveBug;
	isBearObject = isBearObj;
}

EWeaponType AMovement::GetCurrentWeaponType()
{
	return g_Projectile_Type;
}

bool AMovement::GetAimingState()
{
	return isAiming;
}

bool AMovement::GetRunningState()
{
	return isRunning;
}

AActor* AMovement::GetThrowProjectile()
{
	return shootComponent->GetActorWrench();
}

bool AMovement::GetPauseState()
{
	return pauseMenuOpen;
}

bool AMovement::GetWaitingState()
{
	return isWaitingWrench;
}

void AMovement::SetUnPause()
{
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	pauseMenuOpen = false;
}

void AMovement::SetUnwaitingState()
{
	isWaitingWrench = false;
}

//UAbilitySystemComponent* AMovement::GetAbilitySystemComponent() const
//{
//	return AbilityComponent;
//}