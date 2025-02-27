// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class USpringArmComponent;
class UCharacterMovementComponent;

struct FInputActionValue;

enum ECurrentWeaponType
{
	AR,
	HG,
	GL
};

UCLASS()
class SEVENDAYS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	float NormalSpeed = 400.0f;
	float SprintSpeed = 700.0f;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkeletalMesh")
	USkeletalMeshComponent* FullBodyMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkeletalMesh")
	USkeletalMeshComponent* FPSMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpringArm")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float Health = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealth = 100.0f;

protected:
	//테스트용
	//TODO: 나중에 불필요시 제거
	// 추후에 무기 클래스 내의 변수를 이용하기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TEST|Status")
	bool bIsReloading = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TEST|Status")
	bool bIsChangingWeapon = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TEST|Status")
	bool bIsBulletEmpty = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TEST|Status")
	bool bIsFiring = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TEST|Status")
	float ChangeWeaponTime = 2.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TEST|Weapon")
	float AR_ReloadTime = 2.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TEST|Weapon")
	float AR_FireRate = 0.1f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TEST|Weapon")
	int32 AR_CurrentBullet = 30;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TEST|Weapon")
	int32 AR_MaxBullet = 30;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TEST|Weapon")
	float HG_ReloadTime = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TEST|Weapon")
	float HG_FireRate = 2.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TEST|Weapon")
	int32 HG_CurrentBullet = 6;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TEST|Weapon")
	int32 HG_MaxBullet = 6;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TEST|Weapon")
	float GL_ReloadTime = 3.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TEST|Weapon")
	float GL_FireRate = 3.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TEST|Weapon")
	int32 GL_CurrentBullet = 3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TEST|Weapon")
	int32 GL_MaxBullet = 3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TEST|Weapon")
	float Current_reloadTime = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TEST|Weapon")
	float Current_fireRate = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TEST|Weapon")
	int32 Current_currentBullet = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TEST|Weapon")
	int32 Current_maxBullet = 0;

	UFUNCTION()
	void Move(const FInputActionValue& _Value);
	UFUNCTION()
	void Look(const FInputActionValue& _Value);
	UFUNCTION()

	void StartJump(const FInputActionValue& _Value);
	UFUNCTION()
	void StopJump(const FInputActionValue& _Value);

	UFUNCTION()
	void StartSprint(const FInputActionValue& _Value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& _Value);

	UFUNCTION()
	void StartCrouch(const FInputActionValue& _Value);
	UFUNCTION()
	void StopCrouch(const FInputActionValue& _Value);

	UFUNCTION()
	void Fire(const FInputActionValue& _Value);

	UFUNCTION()
	void Reload(const FInputActionValue& _Value);

	UFUNCTION()
	void ChangeToAR(const FInputActionValue& _Value);

	UFUNCTION()
	void ChangeToHG(const FInputActionValue& _Value);

	UFUNCTION()
	void ChangeToGL(const FInputActionValue& _Value);

	UFUNCTION()
	void WheelUp(const FInputActionValue& _Value);

	UFUNCTION()
	void WheelDown(const FInputActionValue& _Value);



	ECurrentWeaponType CurrentWeaponType = ECurrentWeaponType::AR;

	void SaveWeaponInfo();

	void OnDeath(); //죽었을 때
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override; //데미지를 받았을 때
	
	FTimerHandle FireTimerHandle;
	void EnableFire(); // 발사 가능

	FTimerHandle ReloadTimerHandle;
	void CompleteReloading(); // 재장전 가능

	FTimerHandle ChangeWeaponTimerHandle;
	FTimerDelegate ChangeWeaponARDelegate;
	FTimerDelegate ChangeWeaponHGDelegate;
	FTimerDelegate ChangeWeaponGLDelegate;

	void CompleteChangeWeapon(ECurrentWeaponType _EType); // 무기 변경 가능
};
