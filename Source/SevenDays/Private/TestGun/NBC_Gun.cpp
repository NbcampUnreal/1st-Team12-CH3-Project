// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGun/NBC_Gun.h"

UNBC_Gun::UNBC_Gun()
{
}

//총 발사
int32 UNBC_Gun::Shot()
{
	

	return Damage;
}

void UNBC_Gun::ReLoad()
{
	if (0 < BulletCount)
	{

		//필요한 총알 개수 // 30 - 22  = 8 개 
		int32 necessaryBullet = BulletMaxCount - CurrentBulletCount;
		//반환할 총알
		int32 bullet = BulletCount > necessaryBullet ?
			BulletCount - necessaryBullet : BulletCount;
		//총알 개수 채우기
		CurrentBulletCount = FMath::Clamp(bullet , 0, BulletMaxCount);
	}
}


