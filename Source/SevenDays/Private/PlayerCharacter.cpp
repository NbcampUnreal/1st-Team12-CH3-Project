// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"

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
		UE_LOG(LogTemp, Warning, TEXT("Fire"));
	}
}

void APlayerCharacter::Reload(const FInputActionValue& _Value)
{
	if (_Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Reload"));
	}
}

void APlayerCharacter::ChangeToAR(const FInputActionValue& _Value)
{
	if (_Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("ChangeToAR"));
	}
}

void APlayerCharacter::ChangeToHG(const FInputActionValue& _Value)
{
	if (_Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("ChangeToHG"));
	}
}

void APlayerCharacter::ChangeToGL(const FInputActionValue& _Value)
{
	if (_Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("ChangeToGL"));
	}
}

void APlayerCharacter::WheelUp(const FInputActionValue& _Value)
{
	if (_Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("WheelUp"));
	}
}

void APlayerCharacter::WheelDown(const FInputActionValue& _Value)
{
	if (_Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("WheelDown"));
	}
}

void APlayerCharacter::OnDeath()
{
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
