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
	// 현재 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentHp;
	// 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHp;

	// 일반 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NormalSpeed;

	//달리기 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoostSpeed;
	
	// 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Damage;

	// 생성자
	FNBC_ZombieStruct()
		: CurrentHp(100), MaxHp(100), NormalSpeed(200.f), BoostSpeed(240.f), Damage(10) 
	{
	}


	// 설정 생성자
	FNBC_ZombieStruct(int32 Hp, float Speed, int32 damage) : 
		MaxHp(Hp) , 
		NormalSpeed(Speed),
		Damage(damage)
	{
		CurrentHp = MaxHp;
		BoostSpeed = NormalSpeed * 1.2f;
	}	
};