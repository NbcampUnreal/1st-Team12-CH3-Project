// Fill out your copyright notice in the Description page of Project Settings.


#include "NBC_Zombie_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"


void ANBC_Zombie_AIController::BeginPlay()
{
	if (bIsBoss == false)
	{
		StartGame();
	}
}

void ANBC_Zombie_AIController::StartGame()
{
	bIsStart = true;

	APawn* ControlledPawn = GetPawn();

	if (ControlledPawn)
	{
		UBehaviorTree* BTAsset = Cast<UBehaviorTree>(ControlledPawn->GetClass()->ClassGeneratedBy);

		if (BTAsset)
		{
			RunBehavior(BTAsset);
		}
	}
}



void ANBC_Zombie_AIController::RunBehavior(UBehaviorTree* BTAsset)
{
	if (BTAsset)
	{
		UseBlackboard(BTAsset->BlackboardAsset, BlackboardComp);
		BehaviorComp->StartTree(*BTAsset);
	}
}
