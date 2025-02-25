// Fill out your copyright notice in the Description page of Project Settings.


#include "NBC_ANS_ZomBie_Attack.h"
#include "ZombieBossAnimInstance.h"
#include "NBC_Zombie_Boss_Character.h"

//공격 히트박스 켜주기
void UNBC_ANS_ZomBie_Attack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp)
	{
		ANBC_Zombie_Base_Character* Zombie = Cast<ANBC_Zombie_Base_Character>(MeshComp->GetOwner());
		if (Zombie)
		{
			Zombie->ZombieAttack();
		}
		UE_LOG(LogTemp, Log, TEXT("Cast ZombieBoss with MeshComp"));

	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Sorry Not Cast ZombieBoss with MeshComp"));
	}

}

//공격 히트 박스 꺼주기

void UNBC_ANS_ZomBie_Attack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animatation)
{

	if (MeshComp)
	{
		ANBC_Zombie_Base_Character* Zombie = Cast<ANBC_Zombie_Base_Character>(MeshComp->GetOwner());
		if (Zombie)
		{
			Zombie->ZombieAttackEnd();
		}
		UE_LOG(LogTemp, Log, TEXT("Cast ZombieBoss with MeshComp"));

	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Sorry Not Cast ZombieBoss with MeshComp"));
	}
}
