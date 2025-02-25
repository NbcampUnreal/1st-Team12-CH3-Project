// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ZombieBossAttackReset.generated.h"

/**
 * 
 */
UCLASS()
class SEVENDAYS_API UZombieBossAttackReset : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	//���� ���� ��Ʈ �ڽ� ���ֱ�
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	

};
