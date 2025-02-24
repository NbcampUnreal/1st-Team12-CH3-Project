// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ZombieBossAnimInstance.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SEVENDAYS_API UZombieBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	
	UFUNCTION()
	void ResetAttackPattern();

	UFUNCTION()
	void ActivePattern(int32 Num);



protected:

	//좀비 애니메이션 여러개일경우 꼬이니 아 이름 설정 잘못했나싶긴하지만
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Zombie")
	int32 NumAttackPattern;
};
