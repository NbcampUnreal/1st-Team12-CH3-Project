// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGun/NBC_Gun.h"

UNBC_Gun::UNBC_Gun()
{
}

//�� �߻�
int32 UNBC_Gun::Shot()
{
	

	return Damage;
}

void UNBC_Gun::ReLoad()
{
	if (0 < BulletCount)
	{

		//�ʿ��� �Ѿ� ���� // 30 - 22  = 8 �� 
		int32 necessaryBullet = BulletMaxCount - CurrentBulletCount;
		//��ȯ�� �Ѿ�
		int32 bullet = BulletCount > necessaryBullet ?
			BulletCount - necessaryBullet : BulletCount;
		//�Ѿ� ���� ä���
		CurrentBulletCount = FMath::Clamp(bullet , 0, BulletMaxCount);
	}
}


