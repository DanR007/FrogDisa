#pragma once

#include "CoreMinimal.h"
#include "MineActor.h"
#include "NonlethalMineActor.generated.h"

/**
 * 
 */
UCLASS()
class FROGDISA_API ANonlethalMineActor : public AMineActor
{
	GENERATED_BODY()
public:

	void Explosion() override;

protected:

private:

	int Time_Before_Explosion;

	bool isReplace = false;
};
