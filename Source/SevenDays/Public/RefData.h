// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RefData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FRefData : public FTableRowBase
{
	GENERATED_BODY()

public:
	// �ش� ��ü�� �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EnemyName;
	// �ּ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<AActor> Enemy;

};
