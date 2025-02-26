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
	// 해당 객체의 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EnemyName;
	// 주소
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<AActor> Enemy;

};
