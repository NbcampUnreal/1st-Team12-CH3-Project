// Fill out your copyright notice in the Description page of Project Settings.


#include "NBC_Zombie_Boss_Character.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "NBC_Zombie_AIController.h"
#include "NBC_ThrowActor.h"
#include "Kismet/GameplayStatics.h"
#include "SevenGameModeBase.h"


ANBC_Zombie_Boss_Character::ANBC_Zombie_Boss_Character()
{
	IsBoss = true;
	ZombieStat = FNBC_ZombieStruct(100, 350, 25, 30);
		

	if (USkeletalMeshComponent* skel = GetMesh())
	{
		SpitPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spit_Point"));
		SpitPoint->SetupAttachment(skel, TEXT("Head"));
	}
}

void ANBC_Zombie_Boss_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//다시
	if (USkeletalMeshComponent* Skel = GetMesh())
	{
		if (UAnimInstance* anim = Skel->GetAnimInstance())
		{
			AnimInstance = Cast<UZombieBossAnimInstance>(anim);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Not cast boss zombie anim"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No have boss zombie anim"));
	}
}

void ANBC_Zombie_Boss_Character::BeginPlay()
{
	Super::BeginPlay();
}

//빙의 될 시
void ANBC_Zombie_Boss_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (ANBC_Zombie_AIController* AIController = Cast<ANBC_Zombie_AIController>(GetController()))
	{
		AIController->Boss();
	}

}

void ANBC_Zombie_Boss_Character::FireProject()
{
	if (SpitObj)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this; // 생성한 캐릭터

		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f; // 캐릭터 앞에서 생성
		FRotator SpawnRotation = GetActorRotation();


		ANBC_ThrowActor* Spit = GetWorld()->SpawnActor<ANBC_ThrowActor>(SpitObj, SpawnLocation, SpawnRotation, SpawnParams);


		//방향설정
		if (Spit) {
			FVector Direction = SpitPoint->GetForwardVector();
			Spit->Direction = Direction;
		}

	}
}

// --------------- 좀비 패턴 -----------------------------

void ANBC_Zombie_Boss_Character::ZombieAttack()
{	
	Super::ZombieAttack();
}

void ANBC_Zombie_Boss_Character::Death()
{
	Super::Death();

	ASevenGameModeBase* Mode = Cast<ASevenGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if
		(Mode)
	{
		Mode->EndCredit();
	}
}



