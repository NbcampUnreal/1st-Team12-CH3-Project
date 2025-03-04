// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Fire.h"

/**
 * 
 */

class SEVENDAYS_API UNBC_Gun : public IFire
{


public:
	UNBC_Gun();

	int32 Shot();

	void ReLoad();

private:
	const float ReloadDelay = 1.0f;
	const float ShotDelay = 0.2f;
	const int32 Damage = 30;

	//���� �Ѿ˰���
	int32 CurrentBulletCount;
	//�Ѿ� ����
	int32 BulletCount;
	//�Ѿ� �ִ� ����
	const int32 BulletMaxCount = 30; 



	
};
