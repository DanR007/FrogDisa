#pragma once

#include "CoreMinimal.h"
#include "WeaponActor.h"
#include "FrogDisa/InteractiveObjectsInterface.h"
#include "WeaponLogicInterface.h"
#include "Components/SphereComponent.h"
#include "NonlethalMineActor.generated.h"

/**
 * 
 */
UCLASS()
class FROGDISA_API ANonlethalMineActor : public AActor, public IWeaponLogicInterface, public IInteractiveObjectsInterface
{
	GENERATED_BODY()
public:
	ANonlethalMineActor();
	virtual bool Launch() override;
	virtual void AttachToCharacter() override;
	virtual void Interact() override;
	virtual void ChangeOutlines_Implementation(bool isOutline) override { UE_LOG(LogTemp, Warning, TEXT("C++")) }

	UFUNCTION(BlueprintCallable)
		bool GetIsReplace() { return isReplace; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* mainMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USphereComponent* ExplosionCollision;

	FCollisionQueryParams queryParams;

	UFUNCTION(BlueprintCallable)
		virtual void Explosion();

protected:

	UFUNCTION(BlueprintNativeEvent)
		void CallExplosionParticle();
private:

	int Time_Before_Explosion;

	bool isReplace = false;
};
