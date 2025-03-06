// Fill out your copyright notice in the Description page of Project Settings.


#include "NBC_Zombie_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"

void ANBC_Zombie_AIController::BeginPlay()
{
	Super::BeginPlay();
		
}

void ANBC_Zombie_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (bIsDaed)
	{
		return;
	}

	if (InPawn && BehaviorTreeAsset)
	{
		if (UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComp))
		{
			UE_LOG(LogTemp, Warning, TEXT("Blackboard initialized successfully"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Blackboard initialization failed!"));
		}
		StartGame();
	}
}

//캐릭터 회전
void ANBC_Zombie_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* TargetActor = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject("PlayerActor"));

	if (TargetActor)
	{
		FVector TargetLocation = TargetActor->GetActorLocation();
		FVector MyLocation = GetPawn()->GetActorLocation();
		FRotator LookAtRotation = (TargetLocation - MyLocation).Rotation();

		GetPawn()->SetActorRotation(FRotator(0, LookAtRotation.Yaw, 0));
	}
}

//생성자
ANBC_Zombie_AIController::ANBC_Zombie_AIController()
{
	// BehaviorComp와 BlackboardComp를 초기화
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	bIsBoss = false;
}

//게임시작
void ANBC_Zombie_AIController::StartGame()
{

	APawn* ControlledPawn = GetPawn();

	if (ControlledPawn)
	{
		UBehaviorTree* BTAsset = BehaviorTreeAsset;

		if (BTAsset)
		{
			RunBehaviorTree(BTAsset);			
		}
	}

	AActor* TarGetActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (GetBlackboardComponent() && TarGetActor) // 블랙보드가 유효한지 확인
	{
		GetBlackboardComponent()->SetValueAsObject("PlayerActor", TarGetActor);
		UE_LOG(LogTemp, Warning, TEXT("Blackboard or PlayerActor"));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Blackboard or PlayerActor is NULL!"));
	}

	
}

void ANBC_Zombie_AIController::Daed()
{
	if (GetBlackboardComponent()) // 블랙보드가 있다면
	{
		GetBlackboardComponent()->SetValueAsBool("IsDead", true); // 블랙보드 값변경
		bIsDaed = true;
	}
}

void ANBC_Zombie_AIController::Boss()
{
	if (GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsBool("IsBoss", true);
		bIsBoss = true;
	}
}




