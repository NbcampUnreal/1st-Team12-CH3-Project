// Fill out your copyright notice in the Description page of Project Settings.


#include "NBC_Zombie_Boss_Character.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "NBC_Zombie_AIController.h"

ANBC_Zombie_Boss_Character::ANBC_Zombie_Boss_Character()
{
	ZombieStat = FNBC_ZombieStruct(1500, 350, 25, 50);
		

	if (USkeletalMeshComponent* skel = GetMesh())
	{
		SpitPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spit_Point"));
		SpitPoint->SetupAttachment(skel, TEXT("Head"));
	}
}

void ANBC_Zombie_Boss_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//다시
	if (USkeletalMeshComponent* Skel = GetMesh())
	{
		if (UAnimInstance* anim = Skel->GetAnimInstance())
		{
			AnimInstance = Cast<UZombieBossAnimInstance>(anim);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Not cast boss zombie anim"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No have boss zombie anim"));
	}
}

void ANBC_Zombie_Boss_Character::BeginPlay()
{
	Super::BeginPlay();
}

//빙의 될 시
void ANBC_Zombie_Boss_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (ANBC_Zombie_AIController* AIController = Cast<ANBC_Zombie_AIController>(GetController()))
	{
		AIController->Boss();
	}

}

// --------------- 좀비 패턴 -----------------------------

void ANBC_Zombie_Boss_Character::ZombieAttack()
{	
	Super::ZombieAttack();
}



