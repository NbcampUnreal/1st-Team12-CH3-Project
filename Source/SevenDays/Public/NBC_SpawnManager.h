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

	//���� �迭���� ������ �ֱ�
	void GetEnemy(const FVector SpawnPoint);

	//���� �迭�� �ֱ�
	UFUNCTION()
	void SetEnemy(AActor* zombie);

	//���� �����ϱ�
	UFUNCTION(BlueprintCallable)
	void CreateZombie(int32 count, const FVector SpawnPoint);

	//���� ���� ���̰ų� �����
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


	//-------------------- �񵿱�� �ҷ��� Ŭ���� �����!!!---------------------
	//  �ε�� Ŭ���� ���� (�ʿ��� �� �����ϱ� ����)

	//���۷��� �����ֱ�
	void SetRef(const FName& RowName);

	//����� �ε� // �񵿱� ������� �ʼ� �ڿ��� �����������

	//----------------���� ��� ���� ----------------------
	//������� ������ �ʿ���� �� ���Ƽ� �̷��� �����.
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> ZombieArr;

	UPROPERTY()
	TArray<AActor*> BossZombie;

	//���� �ִ��
	int32 MaxZombieInex;
	

};
