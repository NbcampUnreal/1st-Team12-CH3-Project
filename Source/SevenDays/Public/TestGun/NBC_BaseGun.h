// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Fire.h"
#include "NBC_BaseGun.generated.h"

/**
 * 
 */
UCLASS()
class SEVENDAYS_API UNBC_BaseGun : public UObject, public IFire
{
	GENERATED_BODY()

public:
	UNBC_BaseGun();

	//움직일때
	void MoveApplyReCoil();

	//멈출때
	void MoveStopReCoil();

	//발사 
	UFUNCTION(BlueprintCallable)
	int32 Shot() override;

	//장전
	UFUNCTION(BlueprintCallable)
	void ReLoad() override;

	//플레이어 지정
	void SetPlayer(AActor* player , APlayerController* PlayerController);

	//반동
	void ApplyRecoli();

	UFUNCTION(BlueprintCallable)
	void Fire() override;

protected:

	AActor* Player;
	APlayerController* PlayerController;
	

private:
	//총마다 바뀔수 있음.
	const float fReloadDelay = 1.0f;
	const float fShotDelay = 0.2f;
	const int32 Damage = 30;
	const float fMaxShotReCoilX = 1.5f;
	const float fMaxShotReCoilY = 5.0f;
	//총알 최대 갯수
	const int32 BulletMaxCount = 120;


	float ShotReCoilX;
	float ShotReCoilY;

	//발사 ,재장전 , 총 반동 타이머
	FTimerHandle ShotTimer;
	FTimerHandle ReLoadTimer;
	FTimerHandle ShotReCoilTimer;

	float ShotDelay;

	float ReloadDelay;

	//딜레이 줄여주기
	void ShotDelayReduction();

	//재장전 딜레이
	void ReloadDelayReduction();

	//총 반동 타이머
	void ReCoilDelayReduction();

	//현재 총알개수
	int32 CurrentBulletCount;

	//총알 개수
	int32 BulletCount;

};
