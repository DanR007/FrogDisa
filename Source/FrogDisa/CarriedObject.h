#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CarriedObjectLogicInterface.h"
#include "InteractiveObjectsInterface.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

#include "CarriedObject.generated.h"


UCLASS()
class FROGDISA_API ACarriedObject : public AActor, public ICarriedObjectLogicInterface, public IInteractiveObjectsInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACarriedObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	//void Interact(AMovement *Player) override;
	void Interact() override{}
	void TakeThisObject();

	void DropThisObject();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

};