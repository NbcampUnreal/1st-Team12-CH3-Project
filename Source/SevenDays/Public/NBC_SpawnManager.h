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
	void GetEnemy(const FVector SpawnPoint);

	//좀비 배열에 넣기
	UFUNCTION()
	void SetEnemy(AActor* zombie);

	//좀비 생성하기
	UFUNCTION(BlueprintCallable)
	void CreateZombie(int32 count, const FVector SpawnPoint);

	//좀비 전부 죽이거나 지우기
	UFUNCTION()
	void ClearZombie();

	UFUNCTION()
	void CreateBoss(const FVector SpawnPoint);



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZOMBIE")
	TSoftClassPtr<AActor> LoadRefClass;


	//-------------------- 비동기로 불러온 클래스 저장용!!!---------------------
	//  로드된 클래스 저장 (필요할 때 스폰하기 위해)

	//레퍼런스 맞춰주기
	void SetRef(const FName& RowName);

	//동기식 로드 // 비동기 방식으로 필수 자원을 사용하지않음

	//----------------좀비가 담길 공간 ----------------------
	//좀비들은 구별할 필요없을 것 같아서 이렇게 사용함.
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> ZombieArr;

	UPROPERTY()
	TArray<AActor*> BossZombie;

	//좀비 최대수
	int32 MaxZombieInex;
	

};
