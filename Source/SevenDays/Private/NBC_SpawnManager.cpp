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

	static ConstructorHelpers::FObjectFinder<UDataTable> ZombieDataTable(TEXT("DataTable'/Game/Zombie/AssetPath.AssetPath'"));

	if (ZombieDataTable.Succeeded())
	{

		static const FString ContextString(TEXT("BaseZombie X")); //알림용이다
			FRefData* Row = ZombieDataTable.Object->FindRow<FRefData>(FName(TEXT("BaseZombie")), ContextString);
			if (Row)
			{
				UE_LOG(LogTemp, Log, TEXT("Successfully loaded class! static"));

				LoadRefClass = Row->Enemy;
			}
			else {
				UE_LOG(LogTemp, Log, TEXT("Failed to load class."));
			}

	}
	else {

	}
	
}

void ANBC_SpawnManager::GetEnemy(const FVector SpawnPoint)
{
	AActor* ResultPawn = nullptr;

	// 클래스가 유효한지 확인
	if (LoadRefClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadedZombieClass is not valid!"));
		return;
	}

	if (!ZombieArr.IsEmpty())
	{
		TWeakObjectPtr<AActor> WeakZombie = ZombieArr.Pop();

		AActor* Zom = WeakZombie.Get();

		if (Zom)
		{
			Zom->SetActorEnableCollision(true);
			Zom->SetActorHiddenInGame(false);
			return;
		}
	}

	UClass* ZombieClass = LoadRefClass.LoadSynchronous();
	if (ZombieClass)
	{
		// 좀비 스폰 타입
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		//소환 코드
		ResultPawn = GetWorld()->SpawnActor<AActor>(ZombieClass,
			FVector::Zero(),
			FRotator::ZeroRotator,
			SpawnParams);
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

// 좀비 배열 받기
void ANBC_SpawnManager::SetEnemy(AActor* zombie)
{
	if (zombie == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("nullptr == zombie"));
		return;
	}

	//콜리전 꺼주고 액터 이미지 꺼주기
	TWeakObjectPtr<AActor> WeakZombie(zombie);
	ZombieArr.Push(zombie);

	zombie->SetActorEnableCollision(false);
	zombie->SetActorHiddenInGame(true);
}

// ---------- 좀비 생성 코드 -----------------
void ANBC_SpawnManager::CreateZombie(int32 count, const FVector SpawnPoint)
{
	for (int32 i = 0; i < count; i++)
	{
		GetEnemy(SpawnPoint);
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

void ANBC_SpawnManager::CreateBoss(const FVector SpawnPoint)
{
	SetRef(FName(TEXT("BossZombie")));
	GetEnemy(SpawnPoint);
}

