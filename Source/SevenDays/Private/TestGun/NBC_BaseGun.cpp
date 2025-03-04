// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGun/NBC_BaseGun.h"
#include "NBC_Zombie_Base_Character.h"
#include "Kismet/GameplayStatics.h"


UNBC_BaseGun::UNBC_BaseGun()
{
	ShotDelay = 0;
	ReloadDelay = 0;
	CurrentBulletCount = 30;
	BulletCount = BulletMaxCount;

	ShotReCoilX = 0;
	ShotReCoilY = 0;
}

void UNBC_BaseGun::MoveApplyReCoil()
{
	ShotReCoilX = FMath::Clamp(ShotReCoilX + 0.1f, 0, fMaxShotReCoilX);
	ShotReCoilY = FMath::Clamp(ShotReCoilY + 0.5f, 0, fMaxShotReCoilY);
}

void UNBC_BaseGun::MoveStopReCoil()
{
	ShotReCoilX = FMath::Clamp(ShotReCoilX - 1.0f, 0, fMaxShotReCoilX);
	ShotReCoilY = FMath::Clamp(ShotReCoilY - 1.0f, 0, fMaxShotReCoilY);
}


// 총 발사
int32 UNBC_BaseGun::Shot( )
{
	if (Player == nullptr)
		return;

	/*if (fShotDelay > 0)
	{
		return 0;
	}*/

	UWorld* World = Player->GetWorld();

	if (World)
	{
		FVector StartLocation = Player->GetActorLocation();
		FVector EndLocation = StartLocation + Player->GetActorForwardVector()* 10000.0f;
		FHitResult HitResult;

		FCollisionQueryParams HitRay;
		//유저 무시
		HitRay.AddIgnoredActor(Player);

		bool bHit = GetWorld()->LineTraceSingleByChannel(
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

	//ShotDelay = fShotDelay;
	//Player->GetWorld()->GetTimerManager().SetTimer(ShotTimer, this, &UNBC_BaseGun::ShotDelayReduction , 0.1f, true);

	//총 반동 올려주기
	ShotReCoilX = FMath::Clamp(ShotReCoilX + 0.1f, 0, fMaxShotReCoilX);
	ShotReCoilY = FMath::Clamp(ShotReCoilY + 0.5f, 0, fMaxShotReCoilY);

	return Damage;
}

// 재장전
void UNBC_BaseGun::ReLoad()
{
	if (Player == nullptr)
		return;

	/*if (fReloadDelay > 0)
	{
		return;
	}*/

	ReloadDelay = fReloadDelay;
	//필요한 총알 개수 // 30 - 22  = 8 개 
	int32 necessaryBullet = BulletMaxCount - CurrentBulletCount;


	if (necessaryBullet <= BulletCount)
	{
		//반환할 총알
		//총알 개수 채우기 // 아직 계산 이상할 수 있음.
		int32 bullet = FMath::Min(necessaryBullet, BulletCount);
		CurrentBulletCount += bullet;
		BulletCount -= bullet;
	}

	//Player->GetWorld()->GetTimerManager().SetTimer(ShotTimer, this, &UNBC_BaseGun::ShotDelayReduction, 0.1f, true);
}

//플레이어 지정 및 컨트롤러 지정 //액터로 지정했으면 어땟을까 건매니저
void UNBC_BaseGun::SetPlayer(AActor* player, APlayerController* playerController)
{
	Player = nullptr;
	Player = player;

	PlayerController = nullptr;
	PlayerController = playerController;
}

//총 반동
void UNBC_BaseGun::ApplyRecoli()
{
	if (PlayerController == nullptr) return;

	float ReCoilX = FMath::RandRange(-ShotReCoilX, ShotReCoilX); // 좌우
	float ReCoilY = FMath::RandRange(ShotReCoilY * 0.5f, ShotReCoilY); // 위아래

	PlayerController->AddPitchInput(-ReCoilY);
	PlayerController->AddYawInput(ReCoilX);
}

void UNBC_BaseGun::Fire()
{
	Shot();
	ApplyRecoli();
}

//총알 딜레이
void UNBC_BaseGun::ShotDelayReduction()
{
	ShotDelay -= Player->GetWorld()->DeltaTimeSeconds;

	if (ShotDelay <= 0)
	{
		Player->GetWorld()->GetTimerManager().ClearTimer(ShotTimer);
	}
}


// 재장전 딜레이 
void UNBC_BaseGun::ReloadDelayReduction()
{
	ReloadDelay -= Player->GetWorld()->DeltaTimeSeconds;

	if (ReloadDelay <= 0)
	{
		Player->GetWorld()->GetTimerManager().ClearTimer(ReLoadTimer);
	}
}

//반동 딜레이 줄이기
void UNBC_BaseGun::ReCoilDelayReduction()
{
	if (ShotReCoilX > 0)
	{
		ShotReCoilX -= Player->GetWorld()->DeltaTimeSeconds;
	}

	if (ShotReCoilY > 0)
	{
		ShotReCoilY -= Player->GetWorld()->DeltaTimeSeconds;
	}
}
