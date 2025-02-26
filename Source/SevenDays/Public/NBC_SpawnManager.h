#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NBC_Zombie_Base_Character.h"
#include "NBC_SpawnManager.generated.h"

UCLASS()
class SEVENDAYS_API ANBC_SpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANBC_SpawnManager();

	//좀비 배열에서 꺼내서 주기
	AActor* GetEnemy(const FVector SpawnPoint);

	//좀비 배열에 넣기
	UFUNCTION()
	void SetEnemy(AActor* zombie);

	//좀비 생성하기
	UFUNCTION()
	void CreateZombie(int32 count, const FVector SpawnPoint);

	//좀비 전부 죽이거나 지우기
	UFUNCTION()
	void ClearZombie();




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	//하드 레퍼런스라 *사용안함
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZOMBIE")
	TArray<TSubclassOf<ANBC_Zombie_Base_Character>> ZombiesRef;
	
	//aactor타입의 액터를 가리키는 소프트 래퍼런스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZOMBIE")
	TSoftClassPtr<ACharacter> ZombieRefClass;

	//해당 타입의 클래스를 가리킴
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZOMBIE")
	TSoftClassPtr<AActor> DataRefClass;

	//-------------------- 비동기로 불러온 클래스 저장용!!!---------------------
	//  로드된 클래스 저장 (필요할 때 스폰하기 위해)
	UClass* LoadedZombieClass;

	// 비동기 로드 클래스 
	void LoadClass(TSoftClassPtr<ACharacter> Ref);

	//동기식 로드
	void LoadClassSynchronously(TSoftClassPtr<ACharacter> Ref);

	//----------------좀비가 담길 공간 ----------------------
	TArray<AActor*> ZombieArr;

	TArray<AActor*> BossZombie;

	//좀비 최대수
	int32 MaxZombieInex;
	

};
