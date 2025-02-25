// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NBC_ANS_ZomBie_Attack.generated.h"

/**
 * 
 */
UCLASS()
class SEVENDAYS_API UNBC_ANS_ZomBie_Attack : public UAnimNotifyState
{
	GENERATED_BODY()
	//공격 범위 시작
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		float TotalDuration) override;
	// 공격 범위 끝
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animatation) override;

};
