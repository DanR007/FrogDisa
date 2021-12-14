#include "Movement.h"
#include "PuzzlePyatnashky.h"
#include "SaveGameSystem.h"

#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

#include "FrogDisa/TestPuzzleActor.h"
#include "FrogDisa/InteractiveObject.h"
#include "FrogDisa/PuzzleInteractiveObject.h"
#include "FrogDisa/MyHUD.h"
#include "FrogDisa/LogicPuzzleActor.h"

#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "Blueprint/UserWidget.h"
#include "DrawDebugHelpers.h"
#include "Styling/SlateWidgetStyleContainerInterface.h"

// Sets default values
AMovement::AMovement()
{

	ConstructorHelpers::FClassFinder<ASteamBug> steam_bug_bp(TEXT("/Game/Blueprint/BP_SteamBug"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/Meshes/Animation/jaba.jaba'"));

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	cameraComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("camera"));
	_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("_Camera"));
	ShootComponent = CreateDefaultSubobject<UShootComponent>(TEXT("Shoot"));
	InteractiveComponent = CreateDefaultSubobject<UInteractiveComponent>(TEXT("InteractiveComponent"));
	UpdateGrapplingOrCollectibleActors = CreateDefaultSubobject<UUpdateBillboardComponent>(TEXT("SetActiveBillboardComponent"));

	_Camera->SetupAttachment(cameraComponent, USpringArmComponent::SocketName);;
	_Camera->bUsePawnControlRotation = false;
	//_Camera->SetRelativeLocation(FVector(0.f, 110.f, 80.f));

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	FVector MeshPosition = FVector(0.f, 0.f, -80.f);
	FRotator MeshRotation = FRotator(0.f, 270.f, 0.f);
	GetMesh()->SetSkeletalMesh(mesh.Object);
	GetMesh()->SetRelativeLocationAndRotation(MeshPosition, MeshRotation);

	GetCapsuleComponent()->SetCapsuleRadius(20.f);

	cameraComponent->bUsePawnControlRotation = true;
	cameraComponent->TargetArmLength = 400.f;
	cameraComponent->SetupAttachment(RootComponent);


	SteamBug_ClassBP = steam_bug_bp.Class;

	isGrappling = false;
	MeleeAttackIsActive = false;
	isBearObject = false;
	isAiming = false;
	isRunning = false;
	pauseMenuOpen = false;
	isClimbing = false;
	isWaitingWrench = false;
	

	Collectibles = 0;

	g_Projectile_Type = EP_Wrench;
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
	PlayerInputComponent->BindAxis("Aim", this, &AMovement::Aim);
	PlayerInputComponent->BindAxis("Run", this, &AMovement::Run);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMovement::Attack);

	PlayerInputComponent->BindAction("TakeInteractiveObject", IE_Pressed, this, &AMovement::InteractionWithObject);

	PlayerInputComponent->BindAction("GrapplingHook", IE_Pressed, this, &AMovement::UseGrapplingHook);

	PlayerInputComponent->BindAction("ActionWithSomeObj", IE_Pressed, this, &AMovement::TakeCollectibles);
	PlayerInputComponent->BindAction("ActionWithSomeObj", IE_Pressed, this, &AMovement::ActionWithPuzzleActor);

	PlayerInputComponent->BindAction("PauseMenu", IE_Pressed, this, &AMovement::PauseMenu);

	PlayerInputComponent->BindAction("SwitchProjectileType", IE_Pressed, this, &AMovement::SwitchProjectile);

	PlayerInputComponent->BindAction("SwitchCharacter", IE_Pressed, this, &AMovement::ChangeCharacter);
}

// Called when the game starts or when spawned
void AMovement::BeginPlay()
{
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

		UpdateGrapplingOrCollectibleActors->CheckCollectibleActor();
		UpdateGrapplingOrCollectibleActors->CheckGrapplingPoint(isGrappling, endLoc);
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, UpdateGrapplingOrCollectibleActors->grapplingObject->GetFullName());
		FHitResult hitPoint;

		FVector Start = GetActorLocation();
		FVector End = Start + GetActorForwardVector() * 40.f;

		if (GetWorld()->LineTraceSingleByChannel(hitPoint, Start, End, ECC_Visibility))
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



void AMovement::MoveForward(float Value)
{
	if (Controller && Value != 0.0f)
	{
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
					isClimbing = false;//ñäåëàòü àíèìàöèþ è âñ¸
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

	}
}

void AMovement::MoveRight(float Value)
{
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
		if (isAiming == true)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Shoot"));
			ShootComponent->ThrowProjectile(g_Projectile_Type);
		}
		else
		{
			MeleeAttackIsActive = true;
			GetWorld()->GetTimerManager().SetTimer(GrapplingTimer, this, &AMovement::SetMeleeAttackInactive, 0.01f, false, 0.5f);
		}
	}
	else
	{
		if (InteractiveComponent->IsZeroOverlappingActors())
		{
			isBearObject = false;
			InteractiveComponent->ThrowInteractiveObject(InteractiveObject);
		}
	}
}

void AMovement::StopShoot()
{
	
}

void AMovement::ChangeCharacter()
{
	//delete this when you will make final build
	isHaveSteamBug = true;

	if (isHaveSteamBug && GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FTransform bugTransform = FTransform(GetActorRotation(), Mesh->GetSocketLocation("feet_L") + FVector(70,0,0));
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
	if (UpdateGrapplingOrCollectibleActors->ActorGrapplingPoint)
	{
		isGrappling = true;
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		GetWorld()->GetTimerManager().SetTimer(GrapplingTimer, this, &AMovement::LerpTo, 0.01f, true, 0.f);
	}
}

void AMovement::LerpTo()
{
	if (GetDistanceTo(UpdateGrapplingOrCollectibleActors->ActorGrapplingPoint) <= 70.f)
	{
		isGrappling = false;

		GetWorldTimerManager().ClearTimer(GrapplingTimer);
		GetMovementComponent()->Velocity = FVector::ZeroVector;
		GetMovementComponent()->Velocity.Z = 500.f;
		UpdateGrapplingOrCollectibleActors->ActorGrapplingPoint->SetActiveObject(false);
		UpdateGrapplingOrCollectibleActors->ActorGrapplingPoint = nullptr;
	}
	else
	{
		GetMovementComponent()->Velocity = (UpdateGrapplingOrCollectibleActors->ActorGrapplingPoint->GetActorLocation() - GetActorLocation()) * 5;
	}
}

void AMovement::InteractionWithObject()
{
	if (isBearObject == true)
	{
		if (InteractiveComponent->OverlapOnlyInteractivePuzzle())
		{
			isBearObject = false;
			InteractiveComponent->DropInteractiveObject(InteractiveObject);
		}
	}
	else
	{
		isBearObject = InteractiveComponent->TakeInteractiveObject(InteractiveObject);
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
	if (CharacterMovement->IsFalling() == false)
	{
		bPressedJump = true;
		JumpKeyHoldTime = 0.0f;
	}
}

void AMovement::StopJumping()
{
	bPressedJump = false;
	ResetJumpState();
}

void AMovement::Aim(float Value)
{
	if (Value != 0)
	{
		isAiming = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		_Camera->bUsePawnControlRotation = true;

		cameraComponent->bUsePawnControlRotation = true;

		bUseControllerRotationYaw = true;

		if (cameraComponent->TargetArmLength > 140.f)
			cameraComponent->TargetArmLength -= ChangeTargetArmSpeed;

	}
	else
	{
		isAiming = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		_Camera->bUsePawnControlRotation = false;

		cameraComponent->bUsePawnControlRotation = true;

		bUseControllerRotationYaw = false;

		if (cameraComponent->TargetArmLength < 400.f)
		{
			cameraComponent->TargetArmLength += ChangeTargetArmSpeed;
		}
	}
}

bool AMovement::GetAimingState()
{
	return isAiming;
}

bool AMovement::GetRunningState()
{
	return isRunning;
}

AActor *AMovement::GetThrowProjectile()
{
	return ShootComponent->GetActorWrench();
}

void AMovement::TakeCollectibles()
{
	FHitResult hitPoint;

	if (CanMakeAction())
	{
		FVector Start = _Camera -> GetComponentLocation();
		FVector End = _Camera -> GetForwardVector() * 1000.f + Start;

		if (UpdateGrapplingOrCollectibleActors->ActorCollectibleObject)
		{
			Collectibles++;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(Collectibles));
			UpdateGrapplingOrCollectibleActors->ActorCollectibleObject->Destroy();
			UpdateGrapplingOrCollectibleActors->ActorCollectibleObject = nullptr;
		}
	}
}

void AMovement::ActionWithPuzzleActor()
{
	if (CanMakeAction())
	{
		FHitResult hitPoint;
		FVector Start = _Camera->GetComponentLocation();
		FVector End = _Camera->GetForwardVector() * 1000.f + Start;
		if (GetWorld()->LineTraceSingleByChannel(hitPoint, GetActorLocation(), End, ECC_Visibility, CollisionParams) == true)
		{
			if (Cast<APuzzleActor>(hitPoint.Actor.Get()))
			{
				Cast<APuzzleActor>(hitPoint.Actor.Get())->Use();
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

void AMovement::ForwardTrace()
{
	FHitResult hitPoint;
	FVector f_start, f_end;
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(this);
	f_start = GetActorLocation();
	f_end = GetActorLocation() + GetActorForwardVector() * 100;

	if(GetWorld()->LineTraceSingleByChannel(hitPoint, f_start, f_end, ECC_GameTraceChannel1, collisionParams))
	{
		wallNormal = hitPoint.Normal;
		wallLocation = hitPoint.Location;
	}
}

void AMovement::HeightTrace()
{
	FHitResult hitPoint;
	FVector h_start, h_end;
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(this);
	h_start = GetActorLocation() + FVector(0, 0, 200.f) + GetActorForwardVector() * 75;
	h_end = h_start - FVector(0, 0, 200.f);

	if (GetWorld()->LineTraceSingleByChannel(hitPoint, h_start, h_end, ECC_GameTraceChannel1, collisionParams))
	{
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
	}
}

void AMovement::SwitchProjectile()
{
	g_Projectile_Type == EP_Stone ? g_Projectile_Type = EP_Wrench : g_Projectile_Type = EP_Stone;
}

bool AMovement::CanMakeAction()
{
	return !isBearObject;
}


void AMovement::DetachInteractiveObject()
{
	isBearObject = false;
	InteractiveComponent->DetachInteractiveFromParent(InteractiveObject);
}

bool AMovement::IsHaveASteamBug()
{
	return isHaveSteamBug;
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
	isHaveSteamBug = isHaveBug;
	isBearObject = isBearObj;
}