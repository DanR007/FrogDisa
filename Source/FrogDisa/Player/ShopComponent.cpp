// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopComponent.h"
#include "FrogDisa/DefaultVariables.h"

#include <algorithm>

// Sets default values for this component's properties
UShopComponent::UShopComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
#ifdef TEST
	money = 100000;
#else
	money = 0;
#endif
}


// Called when the game starts
void UShopComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UShopComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UShopComponent::Buy(int cost_object, int count_objects, EWeaponType weapon_type, EObjectType object_type)
{ 
	int true_count;
	if (weapon_type != EWeaponType::EW_None)
	{
		money -= cost_object * count_objects;
		PlayerActor->weaponComponent->AddAmmunition(count_objects, weapon_type);
	}
	else
	{
		true_count = std::min(PlayerActor->HUDComponent->stamina_and_health_objects_map[object_type].second - PlayerActor->HUDComponent->stamina_and_health_objects_map[object_type].first, count_objects);
		money -= true_count * cost_object;
		PlayerActor->HUDComponent->AddHealthObject(true_count, object_type);
	}
}