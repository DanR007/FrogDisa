// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"


AMyHUD::AMyHUD()
{
	ConstructorHelpers::FClassFinder<UUserWidget> HUDWidget(TEXT("/Game/Blueprint/HealthBar"));
	HUDWidgetClass = HUDWidget.Class;
	ConstructorHelpers::FClassFinder<UUserWidget> ShopWidget(TEXT("/Game/Blueprint/ShopWidget"));
	ShopWidgetClass = ShopWidget.Class;
	ConstructorHelpers::FClassFinder<UUserWidget> CircleWidget(TEXT("/Game/Blueprint/CircleMenu"));
	CircleMenuWidgetClass = CircleWidget.Class;
	//InputComponent->

}


void AMyHUD::DrawHUD()
{
	Super::DrawHUD();
}



void AMyHUD::BeginPlay()
{
	Super::BeginPlay();
	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), HUDWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
	
}

void AMyHUD::ShopMenu(bool isShopOpen)
{
	if (ShopWidgetClass)
	{
		if (isShopOpen)
		{
			CurrentWidget->RemoveFromViewport();
			CurrentWidget->Destruct();
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), HUDWidgetClass);

			if (CurrentWidget)
				CurrentWidget->AddToViewport();
		}
		else
		{
			CurrentWidget->RemoveFromViewport();
			CurrentWidget->Destruct();
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), ShopWidgetClass);

			if (CurrentWidget)
				CurrentWidget->AddToViewport();
		}
	}
}