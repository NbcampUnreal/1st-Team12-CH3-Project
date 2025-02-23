// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"



APlayerCharacter::APlayerCharacter()
{
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;

	FullBodyMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FullBodyMeshComponent"));
	FullBodyMeshComponent->SetupAttachment(CapsuleComponent);
	FullBodyMeshComponent->bHiddenInGame = true;
	FullBodyMeshComponent->bCastDynamicShadow = true;
	FullBodyMeshComponent->bCastStaticShadow = true;

	FPSMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSMeshComponent"));
	FPSMeshComponent->SetupAttachment(CapsuleComponent);
	FPSMeshComponent->bHiddenInGame = false;
	FPSMeshComponent->bCastDynamicShadow = true;
	FPSMeshComponent->bCastStaticShadow = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(CapsuleComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	MovementComponent = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("MovementComponent"));
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

