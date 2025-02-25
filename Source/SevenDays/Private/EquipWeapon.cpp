#include "EquipWeapon.h"
#include "Components/SkeletalMeshComponent.h"


AEquipWeapon::AEquipWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	RootComponent = SkeletalMesh;
}

void AEquipWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEquipWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

