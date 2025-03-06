// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NBC_Zombie_Base_Character.h"
#include "ZombieBossAnimInstance.h"
#include "NBC_Zombie_Boss_Character.generated.h"


class ANBC_ThrowActor;

UCLASS()
class SEVENDAYS_API ANBC_Zombie_Boss_Character : public ANBC_Zombie_Base_Character
{
	GENERATED_BODY()
	
public:
	ANBC_Zombie_Boss_Character();


protected:	

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController);

	UFUNCTION(BlueprintCallable)
	void FireProject();

	//애니메이션 인스턴스 (보스용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ANIM")
	UZombieBossAnimInstance* AnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "COMPONENT")
	UArrowComponent* SpitPoint;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spit")
	TSubclassOf<ANBC_ThrowActor> SpitObj;
	//ANBC_ThrowActor* SpitObj;


	virtual void ZombieAttack() override;

	void Death() override;


};
