// Fill out your copyright notice in the Description page of Project Settings.


#include "NBC_SpawnManager.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "RefData.h"

// Sets default values
ANBC_SpawnManager::ANBC_SpawnManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRef(FName(TEXT("BaseZombie")));	
	
}

void ANBC_SpawnManager::SummonEnemy(const FVector SpawnPoint)
{
	AActor* ResultPawn = nullptr;

	// Ŭ������ ��ȿ���� Ȯ��
	if (LoadRefClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadedZombieClass is not valid!"));
		return;
	}

	/*if (!ZombieArr.IsEmpty())
	{
		TWeakObjectPtr<AActor> WeakZombie = ZombieArr.Pop();

		AActor* Zom = WeakZombie.Get();

		if (Zom)
		{
			Zom->SetActorEnableCollision(true);
			Zom->SetActorHiddenInGame(false);
			return;
		}
	}*/

	UClass* ZombieClass = LoadRefClass.LoadSynchronous();
	if (ZombieClass)
	{
		// ���� ���� Ÿ��
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		//��ȯ �ڵ�
		ResultPawn = GetWorld()->SpawnActor<AActor>(ZombieClass,
			SpawnPoint,
			FRotator::ZeroRotator,
			SpawnParams);

		ZombieArr.Add(ResultPawn);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("LoadedZombieClass is not "));

	}
}

// Called when the game starts or when spawned
void ANBC_SpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
	//GetEnemy(FVector::Zero());
}

// Called every frame
void ANBC_SpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANBC_SpawnManager::SetRef(const FName& RowName)
{

	static ConstructorHelpers::FObjectFinder<UDataTable> ZombieDataTable(TEXT("DataTable'/Game/Zombie/AssetPath.AssetPath'"));

	if (ZombieDataTable.Succeeded())
	{
		static const FString ContextString(TEXT("Not Found X")); 
		FRefData* Row = ZombieDataTable.Object->FindRow<FRefData>(RowName, ContextString);
		if (Row)
		{
			UE_LOG(LogTemp, Log, TEXT("Successfully loaded class! static"));

			LoadRefClass = Row->Enemy;
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("Failed to load class."));
		}		
	}
}

// ���� �迭 �ޱ�
//void ANBC_SpawnManager::SetEnemy(AActor* zombie)
//{
//	if (zombie == nullptr)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("nullptr == zombie"));
//		return;
//	}
//
//	//�̷� �Լ��� �ִ��� ó���˾Ҵ� nullptr�ΰ͵��� ���� ���� ���ش�
//	ZombieArr.RemoveAll([](AActor* Z) {return Z == nullptr;});
//
//	//�ݸ��� ���ְ� ���� �̹��� ���ֱ�		
//
//	zombie->SetActorEnableCollision(false);
//	zombie->SetActorHiddenInGame(true);
//
//	ZombieArr.Add(zombie);
//}

// ---------- ���� ���� �ڵ� -----------------
void ANBC_SpawnManager::CreateZombie(int32 count, const FVector SpawnPoint)
{
	SetRef(FName(TEXT("BaseZombie")));

	for (int32 i = 0; i < count; i++)
	{
		SummonEnemy(SpawnPoint);
	}
}

void ANBC_SpawnManager::ClearZombie()
{
	for (AActor* Zombie : ZombieArr)
	{

		if (Zombie)
		{
			Zombie->Destroy();
		}
		/*ZombieArr[i]->SetActorEnableCollision(false);
		ZombieArr[i]->SetActorHiddenInGame(true);*/
		//TArry�� �����Ǵµ� �޸� ���� ��ų�� ������
	}
	ZombieArr.Empty();
}

void ANBC_SpawnManager::CreateBoss(const FVector SpawnPoint)
{
	SetRef(FName(TEXT("BossZombie")));
	SummonEnemy(SpawnPoint);
}

