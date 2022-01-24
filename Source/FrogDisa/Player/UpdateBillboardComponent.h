#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "FrogDisa/GrapplingObject.h"

#include "UpdateBillboardComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FROGDISA_API UUpdateBillboardComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUpdateBillboardComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	const float MinimumGrapplingDistance = 200.f;
	const float MaximumGrapplingDistance = 5000.f;
	const float MaximumCollectibleObjectDistance = 400.f;

public:	

	AActor* ActorTakenObject;


	void CheckTakenObject();
};
