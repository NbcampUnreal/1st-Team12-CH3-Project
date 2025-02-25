#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EquipWeapon.generated.h"

UCLASS()
class SEVENDAYS_API AEquipWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AEquipWeapon();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FDataTableRowHandle WeaponData;
};
