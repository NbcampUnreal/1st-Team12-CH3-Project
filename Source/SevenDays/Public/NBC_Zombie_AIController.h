// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NBC_Zombie_AIController.generated.h"

class UBehaviorTreeComponent;


UCLASS()
class SEVENDAYS_API ANBC_Zombie_AIController : public AAIController
{
	GENERATED_BODY()
	

public:
	ANBC_Zombie_AIController();

	// 만약 생성후 너무 빠르게 접근한다 싶을경우를 대비해 만든 소환용 코드
	UFUNCTION(BlueprintCallable)
	void StartGame();

protected:
	
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value")
	bool bIsStart = false;  // 기본값 설정

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value")
	bool bIsBoss = false;   // 기본값 설정

	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTreeComponent* BehaviorComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBlackboardComponent* BlackboardComp;



};
