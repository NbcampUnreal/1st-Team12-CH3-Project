// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NBC_Zombie_Base_Character.h"
#include "ZombieBossAnimInstance.h"
#include "NBC_Zombie_Boss_Character.generated.h"

/**
 * 
 */


UCLASS()
class SEVENDAYS_API ANBC_Zombie_Boss_Character : public ANBC_Zombie_Base_Character
{
	GENERATED_BODY()
	
public:
	ANBC_Zombie_Boss_Character();

protected:

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ANIM")
	UZombieBossAnimInstance* AnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ANIM")
	UBoxComponent* JumpAttackCollision;

	//���������� �ҷ��� ���� // ��������Ʈó�� �Լ��� ���� �ϴ°� �ִٸ� ������ ����.
	UFUNCTION(BlueprintCallable)
	void ActivePattern(int32 patternNumber);	

	

	UFUNCTION(BlueprintCallable)
	void JumpAttackPattern();

	UFUNCTION(BlueprintCallable)
	void BreathPattern();

};
