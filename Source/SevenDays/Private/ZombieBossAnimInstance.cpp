// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieBossAnimInstance.h"

void UZombieBossAnimInstance::ResetAttackPattern()
{
	NumAttackPattern = 0;
}

void UZombieBossAnimInstance::ActivePattern(int32 Num)
{
	NumAttackPattern = FMath::Clamp(Num, 1, 3);
}
