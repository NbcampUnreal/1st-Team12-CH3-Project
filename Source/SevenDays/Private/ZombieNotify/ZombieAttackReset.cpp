// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieNotify/ZombieAttackReset.h"
#include "NBC_Zombie_Base_Character.h"

void UZombieAttackReset::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		ANBC_Zombie_Base_Character* Zombie
			= Cast<ANBC_Zombie_Base_Character>(MeshComp->GetOwner());
		if (Zombie)
		{
			//Zombie->ZombieAttackEnd();
		}
	}
}
