#include "PickUpWeapon.h"
#include "Components/SphereComponent.h"
#include "Weapon.h"
#include "PlayerCharacter.h"
#include "EquipWeapon.h"

APickUpWeapon::APickUpWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	RootComponent = StaticMesh;
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMesh->SetCollisionObjectType(ECC_WorldDynamic);
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Block);
	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Ignore);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SphereCollision->SetupAttachment(StaticMesh);
	SphereCollision->SetSphereRadius(64.0f);
}

void APickUpWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickUpWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}