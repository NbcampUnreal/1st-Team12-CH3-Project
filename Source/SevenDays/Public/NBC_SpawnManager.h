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
	
	//좀비가 담길 공간
	TArray<AActor*> ZombieArr;

	TArray<AActor*> BossZombie;

	//좀비 최대수
	int32 MaxZombieInex;
	

};
