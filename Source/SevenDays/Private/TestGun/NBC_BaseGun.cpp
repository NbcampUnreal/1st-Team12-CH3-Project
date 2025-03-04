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


// �� �߻�
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
		//���� ����
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
				//������ �ִ� �Լ� ��α� ����
				UGameplayStatics::ApplyDamage(Zombie, 30, Zombie->GetInstigatorController(), Zombie, NULL);
			}
		}
	}

	//ShotDelay = fShotDelay;
	//Player->GetWorld()->GetTimerManager().SetTimer(ShotTimer, this, &UNBC_BaseGun::ShotDelayReduction , 0.1f, true);

	//�� �ݵ� �÷��ֱ�
	ShotReCoilX = FMath::Clamp(ShotReCoilX + 0.1f, 0, fMaxShotReCoilX);
	ShotReCoilY = FMath::Clamp(ShotReCoilY + 0.5f, 0, fMaxShotReCoilY);

	return Damage;
}

// ������
void UNBC_BaseGun::ReLoad()
{
	if (Player == nullptr)
		return;

	/*if (fReloadDelay > 0)
	{
		return;
	}*/

	ReloadDelay = fReloadDelay;
	//�ʿ��� �Ѿ� ���� // 30 - 22  = 8 �� 
	int32 necessaryBullet = BulletMaxCount - CurrentBulletCount;


	if (necessaryBullet <= BulletCount)
	{
		//��ȯ�� �Ѿ�
		//�Ѿ� ���� ä��� // ���� ��� �̻��� �� ����.
		int32 bullet = FMath::Min(necessaryBullet, BulletCount);
		CurrentBulletCount += bullet;
		BulletCount -= bullet;
	}

	//Player->GetWorld()->GetTimerManager().SetTimer(ShotTimer, this, &UNBC_BaseGun::ShotDelayReduction, 0.1f, true);
}

//�÷��̾� ���� �� ��Ʈ�ѷ� ���� //���ͷ� ���������� ����� �ǸŴ���
void UNBC_BaseGun::SetPlayer(AActor* player, APlayerController* playerController)
{
	Player = nullptr;
	Player = player;

	PlayerController = nullptr;
	PlayerController = playerController;
}

//�� �ݵ�
void UNBC_BaseGun::ApplyRecoli()
{
	if (PlayerController == nullptr) return;

	float ReCoilX = FMath::RandRange(-ShotReCoilX, ShotReCoilX); // �¿�
	float ReCoilY = FMath::RandRange(ShotReCoilY * 0.5f, ShotReCoilY); // ���Ʒ�

	PlayerController->AddPitchInput(-ReCoilY);
	PlayerController->AddYawInput(ReCoilX);
}

void UNBC_BaseGun::Fire()
{
	Shot();
	ApplyRecoli();
}

//�Ѿ� ������
void UNBC_BaseGun::ShotDelayReduction()
{
	ShotDelay -= Player->GetWorld()->DeltaTimeSeconds;

	if (ShotDelay <= 0)
	{
		Player->GetWorld()->GetTimerManager().ClearTimer(ShotTimer);
	}
}


// ������ ������ 
void UNBC_BaseGun::ReloadDelayReduction()
{
	ReloadDelay -= Player->GetWorld()->DeltaTimeSeconds;

	if (ReloadDelay <= 0)
	{
		Player->GetWorld()->GetTimerManager().ClearTimer(ReLoadTimer);
	}
}

//�ݵ� ������ ���̱�
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
