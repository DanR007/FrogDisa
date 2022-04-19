// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCPawn.h"
#include "FrogDisa/MyHUD.h"
#include "FrogDisa/DefaultVariables.h"
// Sets default values
ANPCPawn::ANPCPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//skeletal_mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	widget_component = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));

	ConstructorHelpers::FClassFinder<ANPCAIController> controllerClass(TEXT("Class'/Script/FrogDisa.NPCAIController'"));
	controllerSubclass = controllerClass.Class;
	AIControllerClass = controllerSubclass;
}

// Called when the game starts or when spawned
void ANPCPawn::BeginPlay()
{
	Super::BeginPlay();
	npc_controller = Cast<ANPCAIController>(Controller);
	npc_controller->SetOwnerActor(this);
	npc_controller->StartBehaviorTreeFromParent();
	npc_controller->GetBlackBoardComponent()->SetValueAsBool("isDie", false);
	//widget_component->GetUserWidgetObject()
	
}

// Called every frame
void ANPCPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPCPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPCPawn::Interact()
{
	APlayerController* controller = GetWorld()->GetFirstPlayerController();
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Interact called");
	Cast<AMyHUD>(controller->GetHUD())->ShopMenu(false);
	controller->SetShowMouseCursor(true);
	controller->SetPause(true);
}

void ANPCPawn::Die_Implementation()
{

}