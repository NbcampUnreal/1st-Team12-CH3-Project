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

	//�����϶�
	void MoveApplyReCoil();

	//���⶧
	void MoveStopReCoil();

	//�߻� 
	UFUNCTION(BlueprintCallable)
	int32 Shot() override;

	//����
	UFUNCTION(BlueprintCallable)
	void ReLoad() override;

	//�÷��̾� ����
	void SetPlayer(AActor* player , APlayerController* PlayerController);

	//�ݵ�
	void ApplyRecoli();

	UFUNCTION(BlueprintCallable)
	void Fire() override;

protected:

	AActor* Player;
	APlayerController* PlayerController;
	

private:
	//�Ѹ��� �ٲ�� ����.
	const float fReloadDelay = 1.0f;
	const float fShotDelay = 0.2f;
	const int32 Damage = 30;
	const float fMaxShotReCoilX = 1.5f;
	const float fMaxShotReCoilY = 5.0f;
	//�Ѿ� �ִ� ����
	const int32 BulletMaxCount = 120;


	float ShotReCoilX;
	float ShotReCoilY;

	//�߻� ,������ , �� �ݵ� Ÿ�̸�
	FTimerHandle ShotTimer;
	FTimerHandle ReLoadTimer;
	FTimerHandle ShotReCoilTimer;

	float ShotDelay;

	float ReloadDelay;

	//������ �ٿ��ֱ�
	void ShotDelayReduction();

	//������ ������
	void ReloadDelayReduction();

	//�� �ݵ� Ÿ�̸�
	void ReCoilDelayReduction();

	//���� �Ѿ˰���
	int32 CurrentBulletCount;

	//�Ѿ� ����
	int32 BulletCount;

};
