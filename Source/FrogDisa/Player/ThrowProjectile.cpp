// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowProjectile.h"
#include "Kismet/KismetMathLibrary.h"

#include "Movement.h"
#include "FrogDisa/DropItAfterShot.h"

#include "UObject/ConstructorHelpers.h"

#define FIRST

// Sets default values
float currentAlpha;
float currentRightAlpha;

AThrowProjectile::AThrowProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("StaticMesh'/Game/Weapon_Pack/Mesh/Weapons/Weapons_Kit/SM_Axe.SM_Axe'"));
	ProjectileMesh -> SetStaticMesh(mesh.Object);
	ProjectileMesh->SetGenerateOverlapEvents(true);
	ProjectileMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	ProjectileMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	ProjectileMesh->SetupAttachment(RootComponent);

	OnActorBeginOverlap.AddDynamic(this, &AThrowProjectile::OnOverlap);

}

// Called when the game starts or when spawned
void AThrowProjectile::BeginPlay()
{
	Super::BeginPlay();
	OwnerPlayer = GetOwner();
	inAir = false;
	isLaunched = false;
	projectileIsReturning = false;
}

void AThrowProjectile::AttachToCharacter(AActor* player_Character)
{
	OwnerPlayer = player_Character;
	this->AttachToActor(OwnerPlayer, FAttachmentTransformRules::KeepWorldTransform);
	//ProjectileMesh->AttachToComponent(Cast<AMovement>(OwnerPlayer)->GetMesh(),FAttachmentTransformRules::KeepWorldTransform,TEXT("hand_RSocket"));
	SetActorRelativeLocation(FVector(100, 100 , 0));
}

void AThrowProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AThrowProjectile::ReturnToCharacter()
{
	if (projectileIsReturning == false && isLaunched == true)
	{
		StartLocation = GetActorLocation();
		SetActorRotation(GetActorRotation() * -1);
		GetWorldTimerManager().ClearTimer(MoveTimer);
		projectileIsReturning = true;
		inAir = true;
		halfDistance = GetDistanceTo(OwnerPlayer) / 2;
		currentAlpha = 0.f;
		GetWorld()->GetTimerManager().SetTimer(MoveTimer, this, &AThrowProjectile::Move, 0.01f, true, 0.f);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("ReturnSt"));
	}
}

void AThrowProjectile::ReturnStart()
{

}

void AThrowProjectile::Launch()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "ThrowProjectile");
	if (isLaunched == false)
	{
		FCollisionQueryParams queryParamsWrench;
		queryParamsWrench.AddIgnoredActor(OwnerPlayer);
		FHitResult hitPoint;
		const FVector startLocation = Cast<AMovement>(OwnerPlayer)->Camera->GetComponentLocation(), endLocation = Cast<AMovement>(OwnerPlayer)->Camera->GetForwardVector() * 1000 + startLocation;
		if (GetWorld()->LineTraceSingleByChannel(hitPoint, startLocation, endLocation, ECollisionChannel::ECC_Visibility, queryParamsWrench))
		{
			EndLocation = hitPoint.Location;
		}
		else
		{
			EndLocation = endLocation;
		}
		SetActorRotation(Cast<AMovement>(OwnerPlayer)->Camera->GetComponentRotation());

		DetachRootComponentFromParent(true);
		StartLocation = GetActorLocation();
		halfDistance = FVector::Distance(EndLocation, GetActorLocation()) / 2;

		currentAlpha = 0;

		isLaunched = true;
		inAir = true;

		GetWorld()->GetTimerManager().SetTimer(MoveTimer, this, &AThrowProjectile::Move, 0.01f, true, 0.f);
	}
}


void AThrowProjectile::Move()
{
	if (projectileIsReturning)
	{
		
		RuleToMove(1);//1 is a left direction and -1 is a right direction

		if (FVector::Distance(Cast<AMovement>(OwnerPlayer)->GetActorLocation()/*GetMesh()->GetSocketLocation(TEXT("hand_RSocket"))*/, GetActorLocation()) < 7)
		{
			ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			//ProjectileMesh->AttachToActor(OwnerPlayer, FAttachmentTransformRules::KeepWorldTransform);
			this->AttachToActor(OwnerPlayer, FAttachmentTransformRules::KeepWorldTransform);
			//ProjectileMesh->AttachToComponent(Cast<AMovement>(OwnerPlayer)->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("hand_RSocket"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Returned"));
			GetWorldTimerManager().ClearTimer(MoveTimer);
			Cast<AMovement>(OwnerPlayer)->SetUnwaitingState();
			SetActorRotation(OwnerPlayer->GetActorRotation() + FRotator(-15, 0, 0));
			projectileIsReturning = false;
			isLaunched = false;
			inAir = false;
		}
	}
	else
	{
		if (inAir)
		{

			RuleToMove(-1);
			if (FVector::Distance(EndLocation, GetActorLocation()) <= 1.f)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("ReturnSt"));
				ReturnToCharacter();
			}
		}
	}

}

void AThrowProjectile::RuleToMove(int direction)
{
	if (direction == 1)
	{
		EndLocation = OwnerPlayer->GetActorLocation();
		//EndLocation = Cast<AMovement>(OwnerPlayer)->GetMesh()->GetSocketLocation(TEXT("hand_RSocket"));//easing function
	}

#ifdef FIRST

	currentDistance = FVector::Distance(EndLocation, GetActorLocation());
	currentAlpha += 0.01f;

	//currentAlpha = FMath::Abs(halfDistance - currentDistance) / (halfDistance / 2);
	//currentAlpha = FMath::Clamp(currentAlpha, 0.f, 1.f);
	
	FVector nextRightVector = EndLocation - GetActorRightVector() * UKismetMathLibrary::Lerp(halfDistance, 0, currentAlpha) * direction / 2.f;

	FVector nextLoc = UKismetMathLibrary::VInterpTo_Constant(GetActorLocation(), nextRightVector, 0.1f, 200.f);

	SetActorLocation(nextLoc);

	AddActorLocalRotation(FRotator(-10, 0, 0), false);
#else
	currentAlpha += 0.01f;
	currentDistance = FVector::Distance(EndLocation, GetActorLocation());
	if (halfDistance > currentDistance)
		currentRightAlpha += 0.01;
	else
		currentRightAlpha -= 0.01;

	FVector nextRightVector = nextRightVector = GetActorRightVector() * currentRightAlpha * halfDistance / 5 * direction;
	
	FVector nextLoc = UKismetMathLibrary::VEase(StartLocation, EndLocation, currentAlpha, EEasingFunc::EaseIn);
	SetActorLocation(nextLoc + nextRightVector);
#endif
	

	//nextRightVector = GetActorRightVector() * currentRightAlpha * halfDistance / 5 * direction;
	
	
	//
}


void AThrowProjectile::OnOverlap_Implementation(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor)
	{
		bool cast = Cast<ADropItAfterShot>(OtherActor) == nullptr;

		if (cast)
		{
			if(OtherActor != OwnerPlayer)
				AThrowProjectile::ReturnToCharacter();
		}
		else
		{
			if (Cast<ADropItAfterShot>(OtherActor)->isABreakableMesh() == false)
			{
				AThrowProjectile::ReturnToCharacter();
			}
		}
	}
}


bool AThrowProjectile::GetInAirState()
{
	return inAir;
}

AActor* AThrowProjectile::GetOwnerPlayer()
{
	return OwnerPlayer;
}

void AThrowProjectile::Create()
{

}