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

UCLASS()
class SEVENDAYS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

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


protected:
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

};
