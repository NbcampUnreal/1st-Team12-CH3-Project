// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Fire.h"
#include "SevenPlayerController.h"
#include "NBC_BaseGun.generated.h"




USTRUCT(BlueprintType)
struct FGunInformation
{
	GENERATED_BODY()

	//총기 정보가 담긴 구조체

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Stats")
	FString Name = "Gun";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Stats")
	float fReloadDelay = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Stats")
	float fShotDelay = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Stats")
	int32 Damage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Stats")
	float fMaxShotReCoilX = 240.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Stats")
	float fMaxShotReCoilY = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Stats")
	int32 CurrentBullet = 30; // 보유하고 있는 총알 개수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Stats")
	int32 MaxBullet = 30; // 최대 몇발 가능한지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Stats")
	int32 BulletCount = 30; // 총알 보유하고 있는 개수

	// 생성자
	FGunInformation()
	{
	}

	// 생성자
	FGunInformation(float reLoadDelay, float shotDelay, int32 damage, float recoilX, float recoilY)
		: fReloadDelay(reLoadDelay), fShotDelay(shotDelay), Damage(damage), fMaxShotReCoilX(recoilX), fMaxShotReCoilY(recoilY)
	{
	}

	FGunInformation& SetReloadDelay(FString Value)
	{
		Name = Value;
		return *this;
	}

	FGunInformation& SetReloadDelay(float Value) 
	{ 
		fReloadDelay = Value; 
		return *this;
	}

	FGunInformation& SetShotDelay(float Value)
	{ 
		fShotDelay = Value;
		return *this; 
	}

	FGunInformation& SetDamage(int32 Value) 
	{ 
		Damage = Value; 
		return *this;
	}

	FGunInformation& SetMaxRecoil(float X, float Y)
	{
		fMaxShotReCoilX = X;
		fMaxShotReCoilY = Y;
		return *this; 
	}

	FGunInformation& SetBulletInfo(int32 Current, int32 Max)
	{ 
		//최대 총알 갯수 정해주기
		CurrentBullet = Current; 
		MaxBullet = Current;
		BulletCount = Max;
		return *this;
	}

};

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
	virtual int32 Shot() override;

	//장전
	UFUNCTION(BlueprintCallable)
	virtual void ReLoad() override;

	//플레이어 지정
	void SetPlayer(APawn* player , APlayerController* PlayerController);

	//반동
	void ApplyRecoli();

	UFUNCTION(BlueprintCallable)
	virtual void Fire() override;

	//총 반동 타이머
	void ReCoilDelayReduction();

	void ChangeWeapon(EPlayerWeaponType type);

	int32 GetCurrentBullet() const { return FInfomation.CurrentBullet; };
	float GetShotDelay() const { return FInfomation.fShotDelay; };
	float GetReloadDelay() const { return FInfomation.fReloadDelay; };

	void AddMaxBullet(int32 AddBullet);

protected:

	AActor* Player;
	APlayerController* PlayerController;	

	//---- 총 ----
	//총 정보 담긴 구조체 참조값 // 포인터에서 교체함.
	FGunInformation& FInfomation = FRifle;
	
	//총 정보
	FGunInformation FRifle;
	FGunInformation FPistol;

	EPlayerWeaponType Type;


	float ShotReCoilX;
	float ShotReCoilY;

	//발사 ,재장전 , 총 반동 타이머
	FTimerHandle ShotReCoilTimer;

	//현재 총알개수
	int32 CurrentBulletCount;

	//총알 개수
	int32 BulletCount;

	
	//인터페이스로 관리해도 될거같음.
	void UpdateWeaponUI();
};
