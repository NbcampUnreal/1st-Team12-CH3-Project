#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interact.generated.h"

class APlayerCharacter;

UINTERFACE(MinimalAPI, Blueprintable)
class UInteract : public UInterface
{
	GENERATED_BODY()
};


class SEVENDAYS_API IInteract
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void PickUp(class APlayerCharacter* PlayerCharacter) = 0;
};
