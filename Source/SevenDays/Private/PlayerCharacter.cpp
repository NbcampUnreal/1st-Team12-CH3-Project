// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "TimerManager.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "SevenGameModeBase.h"
#include "SevenGameStateBase.h"
#include "SevenUserWidget.h"
#include "TestGun/NBC_BaseGun.h"

APlayerCharacter::APlayerCharacter()
{
	FullBodyMeshComponent = GetMesh();
	FullBodyMeshComponent->SetupAttachment(RootComponent);
	FullBodyMeshComponent->bHiddenInGame = true;
	FullBodyMeshComponent->bCastDynamicShadow = true;
	FullBodyMeshComponent->bCastStaticShadow = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	FPSMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSMeshComponent"));
	FPSMeshComponent->SetupAttachment(SpringArmComponent);
	FPSMeshComponent->bHiddenInGame = false;
	FPSMeshComponent->bCastDynamicShadow = false;
	FPSMeshComponent->bCastStaticShadow = false;
	
	Current_Weapon = NewObject<UNBC_BaseGun>();

	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponComponent"));
	WeaponComponent->SetupAttachment(FPSMeshComponent, TEXT("rifle_socket"));

	bMoveSoundInterval = true;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Current_Weapon->SetPlayer(UGameplayStatics::GetPlayerPawn(GetWorld(), 0), GetWorld()->GetFirstPlayerController());
	GetWorldTimerManager().SetTimer(RecoilTimerHandle, this, &APlayerCharacter::ReduceRecoil, 0.1f, true);

	ArmsAnimInstance = FPSMeshComponent->GetAnimInstance();

	ChangeWeaponARDelegate.BindLambda([this]() { CompleteChangeWeapon(EPlayerWeaponType::AR); });
	ChangeWeaponHGDelegate.BindLambda([this]() { CompleteChangeWeapon(EPlayerWeaponType::Pistol); });
	ChangeWeaponGLDelegate.BindLambda([this]() { CompleteChangeWeapon(EPlayerWeaponType::Grenade); });

	CompleteChangeWeapon(EPlayerWeaponType::AR);
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASevenPlayerController* PlayerController = Cast<ASevenPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StartJump);
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopJump);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StartSprint);
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprint);
			}

			if (PlayerController->CrouchAction)
			{
				EnhancedInput->BindAction(PlayerController->CrouchAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StartCrouch);
				EnhancedInput->BindAction(PlayerController->CrouchAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopCrouch);
			}

			if (PlayerController->FireAction)
			{
				EnhancedInput->BindAction(PlayerController->FireAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Fire);
			}

			if (PlayerController->ReloadAction)
			{
				EnhancedInput->BindAction(PlayerController->ReloadAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Reload);
			}

			if (PlayerController->ChangeARAction)
			{
				EnhancedInput->BindAction(PlayerController->ChangeARAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ChangeToAR);
			}

			if (PlayerController->ChangeHGAction)
			{
				EnhancedInput->BindAction(PlayerController->ChangeHGAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ChangeToHG);
			}

			if (PlayerController->ChangeGLAction)
			{
				EnhancedInput->BindAction(PlayerController->ChangeGLAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ChangeToGL);
			}

			if (PlayerController->ToggleDayNightAction)
			{
				EnhancedInput->BindAction(PlayerController->ChangeGLAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ChangeToGL);
			}

		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& _Value)
{
	if (!Controller)
		return;

	if (bMoveSoundInterval)
	{
		bMoveSoundInterval = false;
		if (GetCharacterMovement()->MaxWalkSpeed > WalkSpeed)
		{
			UGameplayStatics::PlaySoundAtLocation(this, SprintSound, GetActorLocation(), 0.1f);
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(this, WalkSound, GetActorLocation(), 0.1f);
		}
		GetWorldTimerManager().SetTimer(MoveSoundTimerHandle, this, &APlayerCharacter::EnableWalkSound, 0.5f, false);
	}

	const FVector2D MoveInput = _Value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}


void APlayerCharacter::Look(const FInputActionValue& _Value)
{
	FVector2D LookInput = _Value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void APlayerCharacter::StartJump(const FInputActionValue& _Value)
{
	if (_Value.Get<bool>())
	{
		Jump();
	}
}

void APlayerCharacter::StopJump(const FInputActionValue& _Value)
{
	if (!_Value.Get<bool>())
	{
		StopJumping();
	}
}

void APlayerCharacter::StartSprint(const FInputActionValue& _Value)
{
	if (_Value.Get<bool>())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void APlayerCharacter::StopSprint(const FInputActionValue& _Value)
{
	if (!_Value.Get<bool>())
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void APlayerCharacter::StartCrouch(const FInputActionValue& _Value)
{
	if (_Value.Get<bool>())
	{
		Crouch();
	}
}

void APlayerCharacter::StopCrouch(const FInputActionValue& _Value)
{
	if (!_Value.Get<bool>())
	{
		UnCrouch();
	}
}

void APlayerCharacter::Fire(const FInputActionValue& _Value)
{
	if (_Value.Get<bool>())
	{
		if (!bIsFiring && !bIsReloading && !bIsChangingWeapon)
		{
			bIsFiring = true;
			GetWorldTimerManager().SetTimer(FireTimerHandle, this, &APlayerCharacter::EnableFire, AR_FireRate, false);

			if (Current_LeftBullet > 0)
			{
				ArmsAnimInstance->Montage_Play(FireMontage);
				Current_Weapon->Fire();
				Current_LeftBullet--;
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}
			else
			{
				bIsBulletEmpty = true;
				UGameplayStatics::PlaySoundAtLocation(this, EmptyBulletSound, GetActorLocation());
			}
		}
	}
}

void APlayerCharacter::Reload(const FInputActionValue& _Value)
{
	if (_Value.Get<bool>())
	{
		if (!bIsReloading && !bIsChangingWeapon)
		{
			bIsArmsUpDown = true;
			bIsReloading = true;
			UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
			GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &APlayerCharacter::CompleteReloading, AR_ReloadTime, false);
		}
	}
}

void APlayerCharacter::ChangeToAR(const FInputActionValue& _Value)
{
	if (_Value.Get<bool>())
	{
		if (!bIsChangingWeapon) // TODO : 현재 무기가 AR이 아닐 때만 교체 가능하도록 수정
		{
			bIsArmsUpDown = true;
			bIsChangingWeapon = true;

			SaveWeaponInfo();
			GetWorldTimerManager().SetTimer(ChangeWeaponTimerHandle, ChangeWeaponARDelegate, ChangeWeaponTime, false);
		}
	}
}

void APlayerCharacter::ChangeToHG(const FInputActionValue& _Value)
{
	if (_Value.Get<bool>())
	{
		if (!bIsChangingWeapon) // TODO : 현재 무기가 HG가 아닐 때만 교체 가능하도록 수정
		{
			bIsArmsUpDown = true;
			bIsChangingWeapon = true;
			SaveWeaponInfo();
			GetWorldTimerManager().SetTimer(ChangeWeaponTimerHandle, ChangeWeaponARDelegate, ChangeWeaponTime, false);
		}
	}
}

void APlayerCharacter::ChangeToGL(const FInputActionValue& _Value)
{
	if (_Value.Get<bool>())
	{
		if (!bIsChangingWeapon) // TODO : 현재 무기가 GL이 아닐 때만 교체 가능하도록 수정
		{
			bIsArmsUpDown = true;
			bIsChangingWeapon = true;
			SaveWeaponInfo();
			GetWorldTimerManager().SetTimer(ChangeWeaponTimerHandle, ChangeWeaponARDelegate, ChangeWeaponTime, false);

		}
	}
}

void APlayerCharacter::ToggleDayNight(const FInputActionValue& _Value)
{
	if (_Value.Get<bool>())
	{

		APlayerController* PlayerController = nullptr;
		ASevenPlayerController* SevenPlayerController = nullptr;
		USevenUserWidget* SevenHUDWidget = nullptr;

		PlayerController = GetWorld()->GetFirstPlayerController();
		if (!PlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerController == nullptr"))
				return;
		}
		SevenPlayerController = Cast<ASevenPlayerController>(PlayerController);
		if (!SevenPlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("SevenPlayerController == nullptr"))
				return;
		}
		SevenHUDWidget = SevenPlayerController->CurrentWidget;
		if (!SevenHUDWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("SevenHUDWidget == nullptr"))
				return;
		}

		ASevenGameModeBase* GM = Cast<ASevenGameModeBase>(GetWorld()->GetAuthGameMode());
		if (!GM) return;

		SevenPlayerController->bIsNight = !(SevenPlayerController->bIsNight);

		if (SevenPlayerController->bIsNight)
		{
			GM->TestForceNight();
		}
		else
		{
			GM->TestForceDay();
		}

		// 게임 상태에서 좀비 수를 초기화합니다.
		if (ASevenGameStateBase* GS = Cast<ASevenGameStateBase>(GetWorld()->GetGameState<ASevenGameStateBase>()))
		{
			GS->SetTotalZombies(0);
			GS->SetRemainingZombies(0);
		}

		SevenHUDWidget->UpdateDayNightCycle(SevenPlayerController->bIsNight);
	}
}

void APlayerCharacter::SaveWeaponInfo()
{
	switch (CurrentWeaponType)
	{
	case EPlayerWeaponType::AR:
		AR_CurrentBullet = Current_LeftBullet;
		AR_MaxBullet = Current_MaxBullet;
		AR_ReloadTime = Current_reloadTime;
		AR_FireRate = Current_fireRate;
		break;
	case EPlayerWeaponType::Pistol:
		HG_CurrentBullet = Current_LeftBullet;
		HG_MaxBullet = Current_MaxBullet;
		HG_ReloadTime = Current_reloadTime;
		HG_FireRate = Current_fireRate;
		break;
	case EPlayerWeaponType::Grenade:
		GL_CurrentBullet = Current_LeftBullet;
		GL_MaxBullet = Current_MaxBullet;
		GL_ReloadTime = Current_reloadTime;
		GL_FireRate = Current_fireRate;
		break;
	}
}

void APlayerCharacter::OnDeath()
{
	// 카메라 래그돌로 해서 뚝 떨구는 연출 넣어보기
	bIsChangingWeapon = true;
	UE_LOG(LogTemp, Warning, TEXT("OnDeath"));
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);

	if (Health <= 0.0f)
	{
		OnDeath();
	}

	return ActualDamage;
}

void APlayerCharacter::EnableFire()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
	bIsFiring = false;
}

void APlayerCharacter::CompleteReloading()
{
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
	bIsArmsUpDown = false;
	bIsReloading = false;
	bIsBulletEmpty = false;
	UGameplayStatics::PlaySoundAtLocation(this, CompleteReloadSound, GetActorLocation());
	Current_LeftBullet = Current_MaxBullet;
}

void APlayerCharacter::CompleteChangeWeapon(EPlayerWeaponType _EType)
{
	GetWorldTimerManager().ClearTimer(ChangeWeaponTimerHandle);

	bIsArmsUpDown = false;
	bIsChangingWeapon = false;

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (ASevenPlayerController* SPC = Cast<ASevenPlayerController>(PC))
		{
			if (USevenUserWidget* SUW = SPC->CurrentWidget)
			{
				switch (_EType)
				{
				case EPlayerWeaponType::AR:
					Current_reloadTime = AR_ReloadTime;
					Current_fireRate = AR_FireRate;
					Current_LeftBullet = AR_CurrentBullet;
					Current_MaxBullet = AR_MaxBullet;
					SUW->UpdateWeaponUI((TEXT("Assault Rifle")), Current_LeftBullet, Current_MaxBullet);
					break;
				case EPlayerWeaponType::Pistol:
					Current_reloadTime = HG_ReloadTime;
					Current_fireRate = HG_FireRate;
					Current_LeftBullet = HG_CurrentBullet;
					Current_MaxBullet = HG_MaxBullet;
					SUW->UpdateWeaponUI((TEXT("Pistol")), Current_LeftBullet, Current_MaxBullet);
					break;
				case EPlayerWeaponType::Grenade:
					Current_reloadTime = GL_ReloadTime;
					Current_fireRate = GL_FireRate;
					Current_LeftBullet = GL_CurrentBullet;
					Current_MaxBullet = GL_MaxBullet;
					SUW->UpdateWeaponUI((TEXT("Grenade")), Current_LeftBullet, Current_MaxBullet);
					break;
				}

				SUW->UpdateWeaponIcons(_EType); // 선택된 무기 아이콘 업데이트
			}
		}
	}
}

void APlayerCharacter::EnableWalkSound()
{
	bMoveSoundInterval = true;
	GetWorldTimerManager().ClearTimer(MoveSoundTimerHandle);
}

void APlayerCharacter::ReduceRecoil()
{
	Current_Weapon->ReCoilDelayReduction();
}
