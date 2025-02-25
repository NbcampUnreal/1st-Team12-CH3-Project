// Fill out your copyright notice in the Description page of Project Settings.


#include "NBC_SpawnManager.h"

// Sets default values
ANBC_SpawnManager::ANBC_SpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}



// Called when the game starts or when spawned
void ANBC_SpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
	CreateZombie(10);
}

// Called every frame
void ANBC_SpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANBC_SpawnManager::GetEnemy()
{
	if (ZombiesRef.Num() == 0)
		return;

	if (!ZombieArr.IsEmpty())
	{
		//���� �ϰ�� ��� ���� ���ϱ�
		//���⼭ ���� ������ �ش� �׽�Ʈ�� �־����� �ʾƼ� ���� ����
		AActor* Zom = ZombieArr.Pop();
		Zom->SetActorEnableCollision(true);
		Zom->SetActorHiddenInGame(false);
	}
	else {

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		//���� ��ȯ �ε���
		int32 index = FMath::RandRange(0, ZombiesRef.Num() - 2); // ������ ������ �ʵ���

		if (index == ZombieArr.Num()- 1)
		{
			index = ZombieArr.Num() - 1;
			UE_LOG(LogTemp, Log, TEXT("%d"), index);
		}
		else {
			index = FMath::RandRange(0, ZombiesRef.Num() - 1); // ������ ������ �ʵ���
		}

		AActor* ResultPawn = GetWorld()->SpawnActor<AActor>(ZombiesRef[index],
			GetActorLocation(),
			FRotator::ZeroRotator,
			SpawnParams);
	}
}

void ANBC_SpawnManager::SetEnemy(AActor* zombie)
{
	//�ݸ��� ���ְ� ���� �̹��� ���ֱ�
	if (zombie == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("nullptr == zombie"));
		return;
	}

	ZombieArr.Push(zombie);
	zombie->SetActorEnableCollision(false);
	zombie->SetActorHiddenInGame(true);
}

void ANBC_SpawnManager::CreateZombie(int32 count)
{
	for (int32 i = 0; i < count; i++)
	{
		GetEnemy();
	}
}

void ANBC_SpawnManager::ClearZombie()
{
	for (int32 i = 0; i < ZombieArr.Num(); i++)
	{
		ZombieArr[i]->SetActorEnableCollision(false);
		ZombieArr[i]->SetActorHiddenInGame(true);
		//TArry�� �����Ǵµ� �޸� ���� ��ų�� ������
	}
}

