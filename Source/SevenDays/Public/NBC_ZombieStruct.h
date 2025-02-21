// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NBC_ZombieStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FNBC_ZombieStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NormalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoostSpeed;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Damage;

	FNBC_ZombieStruct()
		: CurrentHp(100), MaxHp(100), NormalSpeed(200.f), BoostSpeed(240.f), Damage(10) 
	{
	}



	FNBC_ZombieStruct(int32 Hp, float Speed, int32 damage) : 
		MaxHp(Hp) , 
		NormalSpeed(Speed),
		Damage(damage)
	{
		CurrentHp = MaxHp;
		BoostSpeed = NormalSpeed * 1.2f;
	}	
};