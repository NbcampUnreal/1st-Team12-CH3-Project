// Fill out your copyright notice in the Description page of Project Settings.


#include "NBC_SpawnManager.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

// Sets default values
ANBC_SpawnManager::ANBC_SpawnManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Soft reference를 통해 클래스 경로 지정
	ZombieRefClass = TSoftClassPtr<ACharacter>(FSoftClassPath("/Game/Zombie/BP/BP_NBC_Zombie_Base_Character.BP_NBC_Zombie_Base_Character_C"));


	// 클래스를 동기적으로 로드해서 검증
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
	// 비동기 매니저 가져와서 사용
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(Ref.ToSoftObjectPath(), [this, Ref]()
		{
			// 비동기 로드 완료 후
			if (Ref.IsValid())
			{
				//로드된 클래스 
				LoadedZombieClass = Ref.Get();
				UE_LOG(LogTemp, Log, TEXT("succesed!"));

				//좀비 생성
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

// 좀비 생성 코드
AActor* ANBC_SpawnManager::GetEnemy(const FVector SpawnPoint)
{
	AActor* ResultPawn = nullptr;

	//로드도니 코드 클래스 유효 확인
	if (ZombiesRef.Num() == 0 || LoadedZombieClass == nullptr)
		return nullptr;

	if (!ZombieArr.IsEmpty())
	{
		//보스 일경우 어떻게 할지 정하기
		//여기서 좀비 생성시 해당 테스트를 넣어주지 않아서 생긴 오류
		AActor* Zom = ZombieArr.Pop();
		Zom->SetActorEnableCollision(true);
		Zom->SetActorHiddenInGame(false);
	}
	else {

		// 좀비 스폰 타입
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		//몬스터 소환 인덱스
		int32 index = FMath::RandRange(0, ZombiesRef.Num() - 2); // 보스는 나오지 않도록

		if (index == ZombieArr.Num()- 1)
		{
			index = ZombieArr.Num() - 1;
			UE_LOG(LogTemp, Log, TEXT("%d"), index);
		}
		else {
			index = 0; // 보스는 나오지 않도록
		}

		//소환 코드
		ResultPawn = GetWorld()->SpawnActor<AActor>(LoadedZombieClass,
			SpawnPoint,
			FRotator::ZeroRotator,
			SpawnParams);
		//
	}

	return ResultPawn;

}

// 좀비 배열 받기
void ANBC_SpawnManager::SetEnemy(AActor* zombie)
{
	//콜리전 꺼주고 액터 이미지 꺼주기
	if (zombie == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("nullptr == zombie"));
		return;
	}

	ZombieArr.Push(zombie);
	zombie->SetActorEnableCollision(false);
	zombie->SetActorHiddenInGame(true);
}

// ---------- 좀비 생성 코드 -----------------
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
		//TArry로 관리되는데 메모리 삭제 시킬지 생각중
	}
}

