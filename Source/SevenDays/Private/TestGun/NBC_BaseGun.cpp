// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGun/NBC_BaseGun.h"
#include "NBC_Zombie_Base_Character.h"
#include "Kismet/GameplayStatics.h"
#include "SevenUserWidget.h"
#include "SevenPlayerController.h"


UNBC_BaseGun::UNBC_BaseGun()
{
	//총기 관련 셋팅
	FPistol.SetReloadDelay(2.0f)
		.SetShotDelay(0.1f)
		.SetDamage(25)
		.SetMaxRecoil(2.0f, 5.0f)
		.SetBulletInfo(30, 150);

	FRifle.SetReloadDelay(1.0f)
		.SetShotDelay(2.0f)
		.SetDamage(20)
		.SetMaxRecoil(1.5f, 4.0f)
		.SetBulletInfo(6, 30);

	FInfomation = FRifle;

	Type = EPlayerWeaponType::AR;

	ShotReCoilX = 0;
	ShotReCoilY = 0;
}

void UNBC_BaseGun::MoveApplyReCoil()
{
	ShotReCoilX = FMath::Clamp(ShotReCoilX + 0.1f, 0, FInfomation.fMaxShotReCoilX);
	ShotReCoilY = FMath::Clamp(ShotReCoilY + 0.5f, 0, FInfomation.fMaxShotReCoilY);
}

void UNBC_BaseGun::MoveStopReCoil()
{
	ShotReCoilX = FMath::Clamp(ShotReCoilX - 1.0f, 0, FInfomation.fMaxShotReCoilX);
	ShotReCoilY = FMath::Clamp(ShotReCoilY - 1.0f, 0, FInfomation.fMaxShotReCoilY);
}


// 총 발사
int32 UNBC_BaseGun::Shot( )
{
	if (Player == nullptr)
		return 0;	

	UWorld* World = Player->GetWorld();

	if (World)
	{
		FVector StartLocation = Player->GetActorLocation();
		FVector EndLocation = StartLocation + Player->GetActorForwardVector()* 10000.0f;
		FHitResult HitResult;

		FCollisionQueryParams HitRay;
		//유저 무시
		HitRay.AddIgnoredActor(Player);

		bool bHit = World->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			ECC_Pawn,
			HitRay
		);

		if (bHit)
		{
			ANBC_Zombie_Base_Character* Zombie = Cast<ANBC_Zombie_Base_Character>(HitResult.GetActor());
			if (Zombie)
			{
				//데미지 주는 함수 블로그 참조
				UGameplayStatics::ApplyDamage(Zombie, 30, Zombie->GetInstigatorController(), Zombie, NULL);
			}
		}
	}

	//총 반동 올려주기 // 필요시 총타입 마다 반동 다르게 가능
	ShotReCoilX = FMath::Clamp(ShotReCoilX + 0.1f, 0, FInfomation.fMaxShotReCoilX);
	ShotReCoilY = FMath::Clamp(ShotReCoilY + 0.5f, 0, FInfomation.fMaxShotReCoilY);

	//총알 감소
	FInfomation.CurrentBullet--;

	return FInfomation.Damage;
}

// 재장전
void UNBC_BaseGun::ReLoad()
{
	//플레이어 체크
	if (Player == nullptr)
		return;

	
	//필요한 총알 개수 // 30 - 22  = 8 개 
	int32 necessaryBullet = FMath::Clamp( FInfomation.BulletCount - FInfomation.CurrentBullet
		,0
		,FInfomation.MaxBullet);



	if (necessaryBullet <= FInfomation.BulletCount)
	{
		//반환할 총알
		//총알 개수 채우기 // 아직 계산 이상할 수 있음.
		int32 bullet = FMath::Min(necessaryBullet, FInfomation.BulletCount);
		FInfomation.CurrentBullet += bullet;
		FInfomation.BulletCount -= bullet;
	}

}

//플레이어 지정 및 컨트롤러 지정 //액터로 지정했으면 어땟을까 건매니저
void UNBC_BaseGun::SetPlayer(APawn* player, APlayerController* playerController)
{	
	Player = player;

	PlayerController = playerController;
}

//총 반동
void UNBC_BaseGun::ApplyRecoli()
{
	if (PlayerController == nullptr) return;

	float ReCoilX = FMath::RandRange(-ShotReCoilX, ShotReCoilX); // 좌우
	float ReCoilY = FMath::RandRange(ShotReCoilY * 0.2f, ShotReCoilY); // 위아래

	UE_LOG(LogTemp, Warning, TEXT("--------------  %f"), ReCoilX);
	UE_LOG(LogTemp, Warning, TEXT("||||||||||||||  %f"), ReCoilY);

	PlayerController->AddPitchInput(-ReCoilY);
	PlayerController->AddYawInput(ReCoilX);
}

//발사 
void UNBC_BaseGun::Fire()
{
	if (FInfomation.CurrentBullet <= 0)
		return;

	Shot();
	ApplyRecoli();

	UpdateWeaponUI();
}

//반동 딜레이 줄이기
void UNBC_BaseGun::ReCoilDelayReduction()
{
	ShotReCoilX = FMath::Clamp(ShotReCoilX - Player->GetWorld()->DeltaTimeSeconds, 0, FInfomation.fMaxShotReCoilX);

	ShotReCoilY = FMath::Clamp(ShotReCoilY - Player->GetWorld()->DeltaTimeSeconds, 0, FInfomation.fMaxShotReCoilY);

}

//무기 교체 함수 
void UNBC_BaseGun::ChangeWeapon(EPlayerWeaponType type)
{
	switch (type)
	{
	case EPlayerWeaponType::AR:
		FInfomation = FRifle;
		break;
	case EPlayerWeaponType::Pistol:
		FInfomation = FPistol;
		break;
	case EPlayerWeaponType::Grenade:
		break;
	default:
		break;
	}

	UpdateWeaponUI();

	Type = type;
}

void UNBC_BaseGun::UpdateWeaponUI()
{
	ASevenPlayerController* controller =
		Cast<ASevenPlayerController>(PlayerController);
	controller->CurrentWidget->UpdateWeaponUI(TEXT("{0}", FInfomation.Name), FInfomation.CurrentBullet, FInfomation.BulletCount);
}

