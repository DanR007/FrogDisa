// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Styling/SlateWidgetStyleContainerInterface.h"
#include "Player/Movement.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class FROGDISA_API AMyHUD : public AHUD
{
	GENERATED_BODY()
public:
	AMyHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

protected:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;
	UUserWidget* CurrentWidget;
	TSubclassOf<UUserWidget> HUDWidgetClass;
};
