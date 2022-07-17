#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveObjectsInterface.h"


class FROGDISA_API InteractiveClass
{
public:
	InteractiveClass() = default;
	~InteractiveClass() = default;
	UPROPERTY(EditDefaultsOnly)
		EInteractionTypes interactive_type;
};
