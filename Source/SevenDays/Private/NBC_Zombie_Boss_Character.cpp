// Fill out your copyright notice in the Description page of Project Settings.


#include "NBC_Zombie_Boss_Character.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"

ANBC_Zombie_Boss_Character::ANBC_Zombie_Boss_Character()
{
	if (USkeletalMeshComponent* skel = GetMesh())
	{
		SpitPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spit_Point"));
		SpitPoint->SetupAttachment(skel);
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
void ANBC_Zombie_Boss_Character::ActivePattern(int32 patternNumber)
{
	//애니메이션 인스턴스확인 후 패턴 설정 
	//해당 코드는  BT에서 불려집니다.
	if (AnimInstance)
		AnimInstance->ActivePattern(patternNumber);
	else {
		UE_LOG(LogTemp, Log, TEXT("NoAnim"));
	}
}

void ANBC_Zombie_Boss_Character::ZombieAttack()
{	
	Super::ZombieAttack();
}




void ANBC_Zombie_Boss_Character::BreathPattern()
{
	
}



