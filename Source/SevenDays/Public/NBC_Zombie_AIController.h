// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NBC_Zombie_AIController.generated.h"

/**
 * 
 */
UCLASS()
class SEVENDAYS_API ANBC_Zombie_AIController : public AAIController
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable)
	void StartGame();

protected:
	
	// 게임 시작
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value")
	bool bIsStart;

};
