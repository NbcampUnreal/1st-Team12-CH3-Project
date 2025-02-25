// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NBC_ZombieStruct.h"
#include "NBC_Zombie_Base_Character.generated.h"

class UBoxComponent;

UCLASS()
class SEVENDAYS_API ANBC_Zombie_Base_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANBC_Zombie_Base_Character();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* HeadCollision;

	//공격 범위
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* HitCollision;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	virtual void ZombieAttack();
	UFUNCTION(BlueprintCallable)
	virtual void ZombieAttackEnd();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FNBC_ZombieStruct ZombieStat;

	UFUNCTION()
	virtual void CollisionOnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);

	FTimerHandle HitDelayTimer;

};
