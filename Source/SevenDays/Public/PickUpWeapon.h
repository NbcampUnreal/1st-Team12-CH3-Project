#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interact.h"
#include "PickUpWeapon.generated.h"

class USphereComponent;

UCLASS()
class SEVENDAYS_API APickUpWeapon : public AActor, public IInteract
{
	GENERATED_BODY()
	
public:	
	APickUpWeapon();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	USphereComponent* SphereCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FDataTableRowHandle WeaponData;

	virtual void PickUp(APlayerCharacter* PlayerCharacter) override;
};
