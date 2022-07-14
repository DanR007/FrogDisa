// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCPawn.h"
#include "FrogDisa/MyHUD.h"
#include "FrogDisa/DefaultVariables.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
ANPCPawn::ANPCPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//skeletal_mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	widget_component = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));

	ConstructorHelpers::FClassFinder<ANPCAIController> controllerClass(TEXT("Class'/Script/FrogDisa.NPCAIController'"));
	AIControllerClass = controllerClass.Class;
	controllerSubclass = controllerClass.Class;
	//AIControllerClass = controllerSubclass;
}

// Called when the game starts or when spawned
void ANPCPawn::BeginPlay()
{
	Super::BeginPlay();
	npc_controller = Cast<ANPCAIController>(Controller);
	if (npc_controller)
	{
		npc_controller->SetOwnerActor(this);
		npc_controller->StartBehaviorTreeFromParent();
		npc_controller->GetBlackBoardComponent()->SetValueAsBool("isDie", false);
	}

	HealthPoint = MaxHealthPoint;
	//widget_component->GetUserWidgetObject()
	
}

// Called every frame
void ANPCPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->Velocity != FVector::ZeroVector)
	{
		////GEngine->AddOnScreenDebugMessage(-1, 0.1, FColor::Green, GetCharacterMovement()->Velocity.ToCompactString());
		SetActorRotation(GetCharacterMovement()->Velocity.Rotation());
	}
}

// Called to bind functionality to input
void ANPCPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPCPawn::Interact()
{
	APlayerController* controller = GetWorld()->GetFirstPlayerController();
	Cast<AMyHUD>(controller->GetHUD())->ShopMenu(false);
	controller->SetShowMouseCursor(true);
	controller->SetPause(true);
}

void ANPCPawn::Die_Implementation()
{

}

void ANPCPawn::GetDamage(int damage)
{
	HealthPoint -= damage;

	if (HealthPoint <= 0)
	{
		Die();
	}
	else
	{
		if(anim_montage_array[0])
			PlayAnimationOnce(anim_montage_array[0]);//Play Hurt Animation
	}
}

void ANPCPawn::Sleep_Implementation()
{

}