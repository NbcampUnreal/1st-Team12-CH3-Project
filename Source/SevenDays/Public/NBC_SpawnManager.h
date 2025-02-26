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
	AActor* GetEnemy(const FVector SpawnPoint);

	//���� �迭�� �ֱ�
	UFUNCTION()
	void SetEnemy(AActor* zombie);

	//���� �����ϱ�
	UFUNCTION()
	void CreateZombie(int32 count, const FVector SpawnPoint);

	//���� ���� ���̰ų� �����
	UFUNCTION()
	void ClearZombie();




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	//�ϵ� ���۷����� *������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZOMBIE")
	TArray<TSubclassOf<ANBC_Zombie_Base_Character>> ZombiesRef;
	
	//aactorŸ���� ���͸� ����Ű�� ����Ʈ ���۷���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZOMBIE")
	TSoftClassPtr<ACharacter> ZombieRefClass;

	//�ش� Ÿ���� Ŭ������ ����Ŵ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZOMBIE")
	TSoftClassPtr<AActor> DataRefClass;

	//-------------------- �񵿱�� �ҷ��� Ŭ���� �����!!!---------------------
	//  �ε�� Ŭ���� ���� (�ʿ��� �� �����ϱ� ����)
	UClass* LoadedZombieClass;

	// �񵿱� �ε� Ŭ���� 
	void LoadClass(TSoftClassPtr<ACharacter> Ref);

	//����� �ε�
	void LoadClassSynchronously(TSoftClassPtr<ACharacter> Ref);

	//----------------���� ��� ���� ----------------------
	TArray<AActor*> ZombieArr;

	TArray<AActor*> BossZombie;

	//���� �ִ��
	int32 MaxZombieInex;
	

};
