// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NBC_Zombie_Base_Character.h"
#include "ZombieBossAnimInstance.h"
#include "NBC_Zombie_Boss_Character.generated.h"


UCLASS()
class SEVENDAYS_API ANBC_Zombie_Boss_Character : public ANBC_Zombie_Base_Character
{
	GENERATED_BODY()
	
public:
	ANBC_Zombie_Boss_Character();

protected:

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	//애니메이션 인스턴스 (보스용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ANIM")
	UZombieBossAnimInstance* AnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "COMPONENT")
	UArrowComponent* SpitPoint;
	

	//실질적으로 불려질 패턴 // 델리게이트처럼 함수를 저장 하는게 있다면 좋을것 같다.
	UFUNCTION(BlueprintCallable)
	void ActivePattern(int32 patternNumber);	

	virtual void ZombieAttack() override;

	// 침 발사
	UFUNCTION(BlueprintCallable)
	void BreathPattern();



};
