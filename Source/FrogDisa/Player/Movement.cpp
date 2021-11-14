// Fill out your copyright notice in the Description page of Project Settings.

#include "Movement.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "Engine/EngineTypes.h"
#include "FrogDisa/InteractiveObject.h"
#include "FrogDisa/Collectibles.h"
#include "FrogDisa/GrapplingObjectComponent.h"
#include "FrogDisa/GrapplingObject.h"
#include "FrogDisa/MyHUD.h"
#include "FrogDisa/MovableObject.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Styling/SlateWidgetStyleContainerInterface.h"
#include "GameFramework/HUD.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/PlayerController.h"
#include "FrogDisa/TestPuzzleActor.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AMovement::AMovement()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->SetCapsuleRadius(20.f);
	cameraComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("camera"));
	cameraComponent->bUsePawnControlRotation = true;
	cameraComponent->TargetArmLength = 400.f;
	//cameraComponent->SocketOffset/*.Y = 110*/ = FVector(0, 110, 80);
	cameraComponent->SetupAttachment(RootComponent);
	cameraOffsetYPlus = FVector(0.f, 80.f, 0.f);
	cameraOffsetYMinus = FVector(0.f, -110.f, 0.f);

	_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("_Camera"));
	_Camera->SetupAttachment(cameraComponent, USpringArmComponent::SocketName);;
	_Camera->bUsePawnControlRotation = false;
	//_Camera->SetRelativeLocation(FVector(0.f, 110.f, 80.f));

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/Meshes/Animation/jaba.jaba'"));
	GetMesh()->SetSkeletalMesh(mesh.Object);
	FVector MeshPosition = FVector(0.f, 0.f, -80.f);
	FRotator MeshRotation = FRotator(0.f, 270.f, 0.f);
	GetMesh()->SetRelativeLocationAndRotation(MeshPosition, MeshRotation);
	//Mesh->An
	ShootComponent = CreateDefaultSubobject<UShootComponent>(TEXT("Shoot"));

	isGrappling = false;
	MeleeAttackIsActive = false;
	isBearObject = false;
	isAiming = false;
	isRunning = false;
	craftMenuOpen = false;
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
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMovement::Attack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AMovement::StopShoot);
	//PlayerInputComponent->BindAction("ReturnWrench", IE_Pressed, this, &AMovement::ReturnWrench);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMovement::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMovement::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AMovement::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AMovement::AddControllerPitchInput);

	PlayerInputComponent->BindAction("TakeInteractiveObject", IE_Pressed, this, &AMovement::InteractionWithObject);
	PlayerInputComponent->BindAction("GrapplingHook", IE_Pressed, this, &AMovement::UseGrapplingHook);
	PlayerInputComponent->BindAction("InteractionWithSomeObj", IE_Pressed, this, &AMovement::TakeCollectibles);

	PlayerInputComponent->BindAxis("Aim", this, &AMovement::Aim);
	PlayerInputComponent->BindAxis("Run", this, &AMovement::Run);

	PlayerInputComponent->BindAction("PauseMenu", IE_Pressed, this, &AMovement::PauseMenu);
	PlayerInputComponent->BindAction("SwitchProjectileType", IE_Pressed, this, &AMovement::SwitchProjectile);
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


	if (isBearObject == false)
	{
		ForwardTrace();
		HeightTrace();

		FHitResult hitPoint;
		FHitResult hitPoint2;
		startLoc = GetActorLocation();
		FVector Start = _Camera->GetComponentLocation();
		FVector End = _Camera->GetForwardVector() * 3000.f + Start;
		//DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 10.f, 5);
		if (GetWorld()->LineTraceSingleByChannel(hitPoint, Start, End, ECC_Visibility,
			CollisionParams))
		{
			if (GetDistanceTo(hitPoint.Actor.Get()) > MinimumGrapplingDistance
				&& GetDistanceTo(hitPoint.Actor.Get()) < MaximumGrapplingDistance
				&& hitPoint.Actor->IsA(AGrapplingObject::StaticClass()))
			{
				ActorGrapplingPoint = Cast<AGrapplingObject>(hitPoint.Actor.Get());
				endLoc = hitPoint.Location;
				ActorGrapplingPoint->SetActiveObject(true);
			}
			else
			{
				if (GetDistanceTo(hitPoint.Actor.Get()) < MaximumCollectibleObject && hitPoint.Actor->IsA(ACollectiblesObject::StaticClass()))
				{
					ActorCollectibleObject = Cast<ACollectiblesObject>(hitPoint.Actor.Get());
					ActorCollectibleObject->SetActiveObject(true);
				}
				else
				{
					if (ActorCollectibleObject)
					{
						ActorCollectibleObject->SetActiveObject(false);
						ActorCollectibleObject = nullptr;
					}
				}
				if (ActorGrapplingPoint && isGrappling == false)
				{
					if (ActorGrapplingPoint)
						ActorGrapplingPoint->SetActiveObject(false);
					ActorGrapplingPoint = nullptr;
				}
			}
		}
		else
		{
			if (isGrappling == false)
			{
				if(ActorGrapplingPoint)
					ActorGrapplingPoint->SetActiveObject(false);
				ActorGrapplingPoint = nullptr;
			}
			if (ActorCollectibleObject)
			{
				ActorCollectibleObject->SetActiveObject(false);
				ActorCollectibleObject = nullptr;
			}
		}
		Start = GetActorLocation();
		End = Start + GetActorForwardVector() * 40.f;
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 10.f, 5);
		if (GetWorld()->LineTraceSingleByChannel(hitPoint, Start, End, ECC_Visibility,
			CollisionParams))
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
					isClimbing = false;//сделать анимацию и всё
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
		}
	}
	else
	{
		isBearObject = false;
		InteractiveActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		InteractiveObject->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		InteractiveObject->SetSimulatePhysics(true);
		InteractiveObject->AddImpulse(_Camera->GetForwardVector() * 1500.f * InteractiveObject->GetMass());
		InteractiveObject = nullptr;
	}
}

void AMovement::StopShoot()
{
	
}

void AMovement::SetMeleeAttackInactive()
{
	MeleeAttackIsActive = false;
}

void AMovement::UseGrapplingHook()
{
	if (ActorGrapplingPoint)
	{
		isGrappling = true;
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		GetWorld()->GetTimerManager().SetTimer(GrapplingTimer, this, &AMovement::LerpTo, 0.01f, true, 0.f);
	}
}

void AMovement::LerpTo()
{
	if (GetDistanceTo(ActorGrapplingPoint) <= 70.f)
	{
		isGrappling = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::SanitizeFloat(GetDistanceTo(ActorGrapplingPoint)));
		if (isGrappling)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("true"));
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, isGrappling ? TEXT("true") : TEXT("false"));
		GetWorldTimerManager().ClearTimer(GrapplingTimer);
		GetMovementComponent()->Velocity = FVector::ZeroVector;
		GetMovementComponent()->Velocity.Z = 500.f;
		ActorGrapplingPoint->SetActiveObject(false);
		ActorGrapplingPoint = nullptr;
	}
	else
	{
		GetMovementComponent()->Velocity = (ActorGrapplingPoint->GetActorLocation() - GetActorLocation()) * 5;
	}
}
void AMovement::LerpToGrapplingPoint(FVector StartLocation, FVector EndLocation, float time)
{
	FVector nextLoc = UKismetMathLibrary::VLerp(StartLocation,
		EndLocation, time);
	nextLoc.Z += 0.1f;
	SetActorLocation(nextLoc);
}
void AMovement::InteractionWithObject()
{

	FHitResult hitPoint;
	
	TArray<AActor*> overlappingActors;

	if(InteractiveObject)
		InteractiveObject->GetOverlappingActors(overlappingActors);

	if (overlappingActors.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Daaaaaaaaaa"));
	}
	bool isZeroOverlappingActors = overlappingActors.Num() == 0;
	
	if (isBearObject == true)
	{
		if (isZeroOverlappingActors)
		{
			isBearObject = false;

			InteractiveActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			InteractiveObject->SetSimulatePhysics(true);
			InteractiveObject->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			InteractiveObject = nullptr;

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Drop this"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, overlappingActors[0]->GetName());
		}
	}
	else
	{
		FVector End = GetActorForwardVector() * 500.f + GetActorLocation();
		DrawDebugLine(GetWorld(), GetActorLocation(), End, FColor::Blue, false, 10.f, 5);
		if (GetWorld()->LineTraceSingleByChannel(hitPoint, GetActorLocation(), End, ECC_Visibility, CollisionParams) == true)
		{
			if (hitPoint.Actor->IsA(AInteractiveObject::StaticClass()))
			{
				isBearObject = true;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Take this"));

				InteractiveActor = hitPoint.Actor.Get();
				InteractiveActor->SetActorLocation(GetActorLocation() + GetActorForwardVector() * 90.f + GetActorUpVector() * 10.f);
				InteractiveActor->SetActorRotation(GetActorRotation());
				InteractiveObject = InteractiveActor->FindComponentByClass<UStaticMeshComponent>();
				InteractiveObject->SetSimulatePhysics(false);//отключаем потому что с физикой объект не двигается
				InteractiveObject->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				InteractiveActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

			}
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
	if (Value != 0 && craftMenuOpen == false)
	{
		isAiming = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		_Camera->bUsePawnControlRotation = true;

		cameraComponent->bUsePawnControlRotation = true;

		//bUseControllerRotationPitch = true;
		bUseControllerRotationYaw = true;
		//bUseControllerRotationRoll = true;

		//cameraComponent->TargetArmLength = 300.f;

		if (cameraComponent->TargetArmLength > 140.f)
			cameraComponent->TargetArmLength -= ChangeTargetArmSpeed;

	}
	else
	{
		isAiming = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		_Camera->bUsePawnControlRotation = false;

		cameraComponent->bUsePawnControlRotation = true;

		//bUseControllerRotationPitch = false;
		bUseControllerRotationYaw = false;
		//bUseControllerRotationRoll = false;

		//cameraComponent->TargetArmLength = 400.f;
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

	if (isBearObject == false)
	{
		FVector Start = _Camera -> GetComponentLocation();
		FVector End = _Camera -> GetForwardVector() * 1000.f + Start;
		//DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 10.f, 5);
		if (ActorCollectibleObject)
		{
			Collectibles++;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(Collectibles));
			ActorCollectibleObject->Destroy();
			ActorCollectibleObject = nullptr;
		}
		else
		{
			if(GetWorld()->LineTraceSingleByChannel(hitPoint, GetActorLocation(), End, ECC_Visibility, CollisionParams) == true)
			{
				if (Cast<ATestPuzzleActor>(hitPoint.Actor.Get()))
				{
					Cast<ATestPuzzleActor>(hitPoint.Actor.Get())->Use();
					//Cast<APuzlePyatnashky>(hitPoint.Actor.Get())->Use();
				}
			}
		}
		
	}
}

void AMovement::ReturnWrench()
{
	isWaitingWrench = true;
	ShootComponent->ReturnProjectile();
}

void AMovement::PauseMenu()
{
	if (craftMenuOpen)
	{
		craftMenuOpen = false;
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
		GetWorld()->GetFirstPlayerController()->SetPause(false);

	}
	else
	{
		craftMenuOpen = true;
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		//GetWorld()->GetFirstPlayerController()->Pause
		
		GetWorld()->GetFirstPlayerController()->SetPause(true);
		
	}
}

bool AMovement::GetPauseState()
{
	return craftMenuOpen;
}

bool AMovement::GetWaitingState()
{
	return isWaitingWrench;
}

void AMovement::SetUnPause()
{
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	craftMenuOpen = false;
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
	//DrawDebugLine(GetWorld(), f_start, f_end, FColor::Blue, false, 10.f, 5);
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
	//DrawDebugLine(GetWorld(), h_start, h_end, FColor::Blue, false, 10.f, 5);
	if (GetWorld()->LineTraceSingleByChannel(hitPoint, h_start, h_end, ECC_GameTraceChannel1, collisionParams))
	{
		float distanceZ = Mesh->GetSocketLocation(TEXT("spineSocket")).Z - hitPoint.Location.Z;
		if (distanceZ <= 0 && distanceZ >= -100.f)
		{
			if (!isClimbing && GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Drop this"));
				isClimbing = true;
				GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
				GetCharacterMovement()->StopMovementImmediately();
			}
		}
	}
	//else
	//{
	//	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	//	GetCharacterMovement()->StopMovementImmediately();
	//}
}

void AMovement::SwitchProjectile()
{
	g_Projectile_Type == EP_Stone ? g_Projectile_Type = EP_Wrench : g_Projectile_Type = EP_Stone;
}