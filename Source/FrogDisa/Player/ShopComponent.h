// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EProjectiles.h"
#include "ShopComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FROGDISA_API UShopComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShopComponent();

	void AddMoney(int money_count) { money += money_count; }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		int GetMoney() { return money; }
	UFUNCTION(BlueprintCallable)
		void Buy(int cost_object, int count_objects, EWeaponType weapon_type, EObjectType object_type); 

private:
	int money;
};
