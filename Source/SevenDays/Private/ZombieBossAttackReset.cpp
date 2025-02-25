// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieBossAttackReset.h"
#include "ZombieBossAnimInstance.h"


void UZombieBossAttackReset::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		UZombieBossAnimInstance* ZombieAnim = Cast<UZombieBossAnimInstance>(MeshComp->GetAnimInstance());
		if (ZombieAnim)
		{
			ZombieAnim->ResetAttackPattern();
		}
	}
}
