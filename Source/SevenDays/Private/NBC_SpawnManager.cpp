// Fill out your copyright notice in the Description page of Project Settings.


#include "NBC_SpawnManager.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

// Sets default values
ANBC_SpawnManager::ANBC_SpawnManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Soft reference�� ���� Ŭ���� ��� ����
	ZombieRefClass = TSoftClassPtr<ACharacter>(FSoftClassPath("/Game/Zombie/BP/BP_NBC_Zombie_Base_Character.BP_NBC_Zombie_Base_Character_C"));


	// Ŭ������ ���������� �ε��ؼ� ����
	if (ZombieRefClass.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Successfully loaded class!"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Failed to load class."));
	}
}

// Called when the game starts or when spawned
void ANBC_SpawnManager::BeginPlay()
{
	Super::BeginPlay();
	

	LoadClass(ZombieRefClass);
	LoadClassSynchronously(ZombieRefClass);
}

// Called every frame
void ANBC_SpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANBC_SpawnManager::LoadClass(TSoftClassPtr<ACharacter> Ref)
{
	// �񵿱� �Ŵ��� �����ͼ� ���
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(Ref.ToSoftObjectPath(), [this, Ref]()
		{
			// �񵿱� �ε� �Ϸ� ��
			if (Ref.IsValid())
			{
				//�ε�� Ŭ���� 
				LoadedZombieClass = Ref.Get();
				UE_LOG(LogTemp, Log, TEXT("succesed!"));

				//���� ����
				CreateZombie(10, GetActorLocation());

			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("fail!"));

			}
		});
}

void ANBC_SpawnManager::LoadClassSynchronously(TSoftClassPtr<ACharacter> Ref)
{
	if (Ref.IsValid())
	{
		LoadedZombieClass = Ref.LoadSynchronous();
		if (LoadedZombieClass)
		{
			CreateZombie(10, GetActorLocation());
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Failed to load class synchronously!"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Invalid class reference."));
	}
}

// ���� ���� �ڵ�
AActor* ANBC_SpawnManager::GetEnemy(const FVector SpawnPoint)
{
	AActor* ResultPawn = nullptr;

	//�ε嵵�� �ڵ� Ŭ���� ��ȿ Ȯ��
	if (ZombiesRef.Num() == 0 || LoadedZombieClass == nullptr)
		return nullptr;

	if (!ZombieArr.IsEmpty())
	{
		//���� �ϰ�� ��� ���� ���ϱ�
		//���⼭ ���� ������ �ش� �׽�Ʈ�� �־����� �ʾƼ� ���� ����
		AActor* Zom = ZombieArr.Pop();
		Zom->SetActorEnableCollision(true);
		Zom->SetActorHiddenInGame(false);
	}
	else {

		// ���� ���� Ÿ��
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
			index = 0; // ������ ������ �ʵ���
		}

		//��ȯ �ڵ�
		ResultPawn = GetWorld()->SpawnActor<AActor>(LoadedZombieClass,
			SpawnPoint,
			FRotator::ZeroRotator,
			SpawnParams);
		//
	}

	return ResultPawn;

}

// ���� �迭 �ޱ�
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

// ---------- ���� ���� �ڵ� -----------------
void ANBC_SpawnManager::CreateZombie(int32 count, const FVector SpawnPoint)
{
	for (int32 i = 0; i < count; i++)
	{
		AActor* A = GetEnemy(SpawnPoint);
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

