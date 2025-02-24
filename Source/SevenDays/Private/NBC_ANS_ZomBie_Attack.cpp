// Fill out your copyright notice in the Description page of Project Settings.


#include "NBC_ANS_ZomBie_Attack.h"
#include "ZombieBossAnimInstance.h"
#include "NBC_Zombie_Boss_Character.h"

void UNBC_ANS_ZomBie_Attack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp)
	{
		//공격 감지 함수 생각할것 // 2025- 02 - 21
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

void UNBC_ANS_ZomBie_Attack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animatation)
{

	if (MeshComp)
	{
		//공격 감지 함수 생각할것 // 2025- 02 - 21
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
