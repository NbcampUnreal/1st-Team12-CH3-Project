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

	FPSMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSMeshComponent"));
	FPSMeshComponent->SetupAttachment(SpringArmComponent);
	FPSMeshComponent->bHiddenInGame = false;
	FPSMeshComponent->bCastDynamicShadow = false;
	FPSMeshComponent->bCastStaticShadow = false;

	ChangeWeaponARDelegate.BindUFunction(this, FName("CompleteChangeWeapon"), ECurrentWeaponType::AR);
	ChangeWeaponHGDelegate.BindUFunction(this, FName("CompleteChangeWeapon"), ECurrentWeaponType::HG);
	ChangeWeaponGLDelegate.BindUFunction(this, FName("CompleteChangeWeapon"), ECurrentWeaponType::GL);
	
	Current_reloadTime = AR_ReloadTime;
	Current_fireRate = AR_FireRate;
	Current_currentBullet = AR_CurrentBullet;
	Current_maxBullet = AR_MaxBullet;
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
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

			if (PlayerController->WheelUpAction)
			{
				EnhancedInput->BindAction(PlayerController->WheelUpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::WheelUp);
			}

			if (PlayerController->WheelDownAction)
			{
				EnhancedInput->BindAction(PlayerController->WheelDownAction, ETriggerEvent::Triggered, this, &APlayerCharacter::WheelDown);
			}
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& _Value)
{
	if (!Controller)
		return;

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
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
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
		if (!bIsFiring && !bIsBulletEmpty && !bIsReloading && !bIsChangingWeapon)
		{
			bIsFiring = true;
			UE_LOG(LogTemp, Warning, TEXT("Excute Fire"));
			GetWorldTimerManager().SetTimer(FireTimerHandle, this, &APlayerCharacter::EnableFire, AR_FireRate, false);

			if (Current_currentBullet > 0)
			{
				Current_currentBullet--;
				UE_LOG(LogTemp, Warning, TEXT("CurrentBullet : %d"), Current_currentBullet);
			}
			else
			{
				bIsBulletEmpty = true;
			}
		}
		else if (bIsFiring) // 발사 중일 때, 발사 중
		{
			UE_LOG(LogTemp, Warning, TEXT("Not Excute Fire, Firing"));
		}
		else if (bIsBulletEmpty) // 총알이 없을 때, 재장전 필요
		{
			UE_LOG(LogTemp, Warning, TEXT("Not Excute Fire, Bullet Empty"));
		}
		else if (bIsReloading) // 재장전 중일 때, 재장전 중
		{
			UE_LOG(LogTemp, Warning, TEXT("Not Excute Fire, Reloading"));
		}
		else if (bIsChangingWeapon) // 무기 교체 중일 때, 무기 교체 중
		{
			UE_LOG(LogTemp, Warning, TEXT("Not Excute Fire, Changing Weapon"));
		}
	}
}

void APlayerCharacter::Reload(const FInputActionValue& _Value)
{
	if (_Value.Get<bool>())
	{
		if (!bIsReloading && !bIsChangingWeapon)
		{
			//재장전시 팔 밑으로 내려서 화면에 안보이게 하기
			UE_LOG(LogTemp, Warning, TEXT("Reload"));
			bIsReloading = true;
			GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &APlayerCharacter::CompleteReloading, AR_ReloadTime, false);
		}
		else if (bIsChangingWeapon) // 무기 교체 중일 때, 무기 교체 중
		{
			UE_LOG(LogTemp, Warning, TEXT("Not Excute Reload, Changing Weapon"));
		}
		else if (bIsReloading) // 재장전 중일 때, 재장전 중
		{
			UE_LOG(LogTemp, Warning, TEXT("Not Excute Reload, Reloading"));
		}
	}
}

void APlayerCharacter::ChangeToAR(const FInputActionValue& _Value)
{
	if (_Value.Get<bool>())
	{
		if (!bIsChangingWeapon) // TODO : 현재 무기가 AR이 아닐 때만 교체 가능하도록 수정
		{
			UE_LOG(LogTemp, Warning, TEXT("ChangeToAR"));
			bIsChangingWeapon = true;
			SaveWeaponInfo();
			GetWorldTimerManager().SetTimer(ChangeWeaponTimerHandle, ChangeWeaponARDelegate, ChangeWeaponTime, false);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Not Excute ChangeToAR, Changing Weapon"));
		}
	}
}

void APlayerCharacter::ChangeToHG(const FInputActionValue& _Value)
{
	if (_Value.Get<bool>())
	{
		if (!bIsChangingWeapon) // TODO : 현재 무기가 HG가 아닐 때만 교체 가능하도록 수정
		{
			UE_LOG(LogTemp, Warning, TEXT("ChangeToHG"));
			bIsChangingWeapon = true;
			SaveWeaponInfo();
			GetWorldTimerManager().SetTimer(ChangeWeaponTimerHandle, ChangeWeaponARDelegate, ChangeWeaponTime, false);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Not Excute ChangeToHG, Changing Weapon"));
		}
	}
}

void APlayerCharacter::ChangeToGL(const FInputActionValue& _Value)
{
	if (_Value.Get<bool>())
	{
		if (!bIsChangingWeapon) // TODO : 현재 무기가 GL이 아닐 때만 교체 가능하도록 수정
		{
			UE_LOG(LogTemp, Warning, TEXT("ChangeToGL"));
			bIsChangingWeapon = true;
			SaveWeaponInfo();
			GetWorldTimerManager().SetTimer(ChangeWeaponTimerHandle, ChangeWeaponARDelegate, ChangeWeaponTime, false);

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Not Excute ChangeToGL, Changing Weapon"));
		}
	}
}

void APlayerCharacter::WheelUp(const FInputActionValue& _Value)
{
	if (_Value.Get<bool>()) // 어케 만들지 고민중
	{
		bIsChangingWeapon = true;
		UE_LOG(LogTemp, Warning, TEXT("WheelUp"));
	}
}

void APlayerCharacter::WheelDown(const FInputActionValue& _Value)
{
	if (_Value.Get<bool>()) // 어케 만들지 고민중
	{
		bIsChangingWeapon = true;
		UE_LOG(LogTemp, Warning, TEXT("WheelDown"));
	}
}

void APlayerCharacter::SaveWeaponInfo()
{
	switch (CurrentWeaponType)
	{
	case ECurrentWeaponType::AR:
		AR_CurrentBullet = Current_currentBullet;
		AR_MaxBullet = Current_maxBullet;
		AR_ReloadTime = Current_reloadTime;
		AR_FireRate = Current_fireRate;
		break;
	case ECurrentWeaponType::HG:
		HG_CurrentBullet = Current_currentBullet;
		HG_MaxBullet = Current_maxBullet;
		HG_ReloadTime = Current_reloadTime;
		HG_FireRate = Current_fireRate;
		break;
	case ECurrentWeaponType::GL:
		GL_CurrentBullet = Current_currentBullet;
		GL_MaxBullet = Current_maxBullet;
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
	bIsReloading = false;
	bIsBulletEmpty = false;
	AR_CurrentBullet = AR_MaxBullet;
}

void APlayerCharacter::CompleteChangeWeapon(ECurrentWeaponType _EType)
{
	GetWorldTimerManager().ClearTimer(ChangeWeaponTimerHandle);
	bIsChangingWeapon = false;
	CurrentWeaponType = _EType;
	switch (CurrentWeaponType)
	{
	case ECurrentWeaponType::AR:
		Current_reloadTime = AR_ReloadTime;
		Current_fireRate = AR_FireRate;
		Current_currentBullet = AR_CurrentBullet;
		Current_maxBullet = AR_MaxBullet;
		break;
	case ECurrentWeaponType::HG:
		Current_reloadTime = HG_ReloadTime;
		Current_fireRate = HG_FireRate;
		Current_currentBullet = HG_CurrentBullet;
		Current_maxBullet = HG_MaxBullet;
		break;
	case ECurrentWeaponType::GL:
		Current_reloadTime = GL_ReloadTime;
		Current_fireRate = GL_FireRate;
		Current_currentBullet = GL_CurrentBullet;
		Current_maxBullet = GL_MaxBullet;
		break;
	}
}
