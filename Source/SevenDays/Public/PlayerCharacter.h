// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SevenUserWidget.h"
#include "PlayerCharacter.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class USpringArmComponent;
class UCharacterMovementComponent;
class UNBC_BaseGun;

struct FInputActionValue;

UCLASS()
class SEVENDAYS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	float WalkSpeed = 400.0f;
	float SprintSpeed = 700.0f;

	bool bMoveSoundInterval = false;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkeletalMesh")
	USkeletalMeshComponent* FullBodyMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkeletalMesh")
	USkeletalMeshComponent* FPSMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpringArm")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponMesh")
	USkeletalMeshComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float Health = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealth = 100.0f;

protected:
	//�׽�Ʈ��
	//TODO: ���߿� ���ʿ�� ����
	// ���Ŀ� ���� Ŭ���� ���� ������ �̿��ϱ�
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
	int32 Current_LeftBullet = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TEST|Weapon")
	int32 Current_MaxBullet = 0;
	//�׽�Ʈ�� ��

	//�ѱ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Gun")
	UNBC_BaseGun* Current_Weapon;
	//�ѱ� ��

	//�ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim|Instance")
	UAnimInstance* ArmsAnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim|Montage")
	UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim|ArmsUPDownStatus")
	float ArmsUpDownValue = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim|ArmsUPDownStatus")
	bool bIsArmsUpDown = false;
	//�ִϸ��̼� ��

	//����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Effcts")
	USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Effcts")
	USoundBase* ReloadSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Effcts")
	USoundBase* CompleteReloadSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Effcts")
	USoundBase* ChangeWeaponSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Effcts")
	USoundBase* EmptyBulletSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Effcts")
	USoundBase* WalkSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Effcts")
	USoundBase* SprintSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Effcts")
	USoundBase* LandSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Effcts")
	USoundBase* DeathSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Effcts")
	USoundBase* HitSound;
	//���� ��

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

	UFUNCTION() // �׽�Ʈ��
	void ToggleDayNight(const FInputActionValue& _Value);

	EPlayerWeaponType CurrentWeaponType = EPlayerWeaponType::AR;

	FTimerHandle FireTimerHandle; // ��ݰ� �����ð�
	FTimerHandle RecoilTimerHandle; // ���������� �ݵ��� ����
	FTimerHandle ReloadTimerHandle; // ������ �ð�
	FTimerHandle ChangeWeaponTimerHandle; // ���� ��ü �ð�

	//���� ��ü�� �Ķ���� ������ ���� ��������Ʈ
	FTimerDelegate ChangeWeaponARDelegate;
	FTimerDelegate ChangeWeaponHGDelegate;
	FTimerDelegate ChangeWeaponGLDelegate;


	FTimerHandle MoveSoundTimerHandle; // �̵� �� �Ҹ� ��ġ�� �ʰ� �ϱ� ���� �����ð�
	void SaveWeaponInfo();

	void OnDeath(); //�׾��� ��
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override; //�������� �޾��� ��
	void EnableFire(); // �߻� ����
	void CompleteReloading(); // ������ ����
	void CompleteChangeWeapon(EPlayerWeaponType _EType); // ���� ���� ����
	void EnableWalkSound(); // �Ҹ� ��� ����
	void ReduceRecoil();
};
