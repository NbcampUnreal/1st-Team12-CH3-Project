// Fill out your copyright notice in the Description page of Project Settings.


#include "NBC_Zombie_Boss_Character.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"

ANBC_Zombie_Boss_Character::ANBC_Zombie_Boss_Character()
{
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

// --------------- 좀비 패턴 -----------------------------

void ANBC_Zombie_Boss_Character::ZombieAttack()
{	
	Super::ZombieAttack();
}



