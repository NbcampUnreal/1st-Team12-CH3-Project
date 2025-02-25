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

	//머리 콜리전
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* HeadCollision;

	//공격 범위
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* HitCollision;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetActorHiddenInGame(bool bNewHidden) override;

public:	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	//공격 범위 껏나 키기
	UFUNCTION(BlueprintCallable)
	virtual void ZombieAttack();
	// 공격 범위 끄기
	UFUNCTION(BlueprintCallable)
	virtual void ZombieAttackEnd();


	void Death();

protected:
	//좀비스텟
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FNBC_ZombieStruct ZombieStat;

	//콜리전함수용
	UFUNCTION()
	virtual void CollisionOnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TEST")
	ANBC_SpawnManager* TestSpawnManager;


};
