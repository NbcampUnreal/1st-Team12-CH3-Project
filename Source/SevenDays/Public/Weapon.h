#pragma once

#include "CoreMinimal.h"
#include "Weapon.generated.h"

class APickUpWeapon;
class AEquipWeapon;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	AssaultRifle    UMETA(DisplayName = "Assault Rifle"),
	Pistol          UMETA(DisplayName = "Pistol"),
	GrenadeLauncher UMETA(DisplayName = "Grenade Launcher")
};

class SEVENDAYS_API Weapon
{
public:
	Weapon();
	~Weapon();
};

USTRUCT(BlueprintType)
struct FS_Weapon : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FName WeaponName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    EWeaponType WeaponType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 MagazineSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<APickUpWeapon> PickupWeapon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AEquipWeapon> EquipWeapon;
};