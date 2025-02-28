// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NBC_ZombieStruct.h"
#include "NBC_Zombie_Base_Character.generated.h"

class UBoxComponent;
class ANBC_SpawnManager;

UCLASS()
class SEVENDAYS_API ANBC_Zombie_Base_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANBC_Zombie_Base_Character();

	//�Ӹ� �ݸ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* HeadCollision;

	//���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* HitCollision;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// ĳ���� ���ӿ��� ���̰� ���� ���� ���ϴ� �ڵ�
	virtual void SetActorHiddenInGame(bool bNewHidden) override;

public:	
	//���� ���� �� �ڵ�
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	//���� ���� ���� Ű��
	UFUNCTION(BlueprintCallable)
	virtual void ZombieAttack();

	

	// ���� ����
	void Death();

protected:
	//������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FNBC_ZombieStruct ZombieStat;


};
