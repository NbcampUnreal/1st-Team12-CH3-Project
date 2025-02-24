// Fill out your copyright notice in the Description page of Project Settings.


#include "NBC_Zombie_Boss_Character.h"


ANBC_Zombie_Boss_Character::ANBC_Zombie_Boss_Character()
{
	
}

void ANBC_Zombie_Boss_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//´Ù½Ã
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

void ANBC_Zombie_Boss_Character::ActivePattern(int32 patternNumber)
{
	if (AnimInstance)
		AnimInstance->ActivePattern(patternNumber);
	else {
		UE_LOG(LogTemp, Log, TEXT("NoAnim"));
	}
}


void ANBC_Zombie_Boss_Character::JumpAttackPattern()
{
	
}

void ANBC_Zombie_Boss_Character::BreathPattern()
{
	
}
