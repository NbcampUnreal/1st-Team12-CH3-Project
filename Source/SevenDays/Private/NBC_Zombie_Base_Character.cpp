// Fill out your copyright notice in the Description page of Project Settings.


#include "NBC_Zombie_Base_Character.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "NBC_SpawnManager.h"
#include "NBC_Zombie_AIController.h"
#include "SevenGameStateBase.h"
#include "SevenGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"




// Sets default values
ANBC_Zombie_Base_Character::ANBC_Zombie_Base_Character():
	ZombieStat(FNBC_ZombieStruct(100, 300, 10))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		HeadCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadCollision"));
		HeadCollision->SetupAttachment(MeshComp, TEXT("Head"));			
	}


	MaxWalkSpeed = 600.0f;
}

// Called when the game starts or when spawned
void ANBC_Zombie_Base_Character::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle Die;

	//GetWorldTimerManager().SetTimer(Die, this, &ANBC_Zombie_Base_Character::Death, 5, false);
}

// Called every frame
void ANBC_Zombie_Base_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANBC_Zombie_Base_Character::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);

	//���������ϴ¼���
	if (!bNewHidden)
	{
		SetActorEnableCollision(true);
		ZombieStat.CurrentHp = ZombieStat.MaxHp;
	}
}

float ANBC_Zombie_Base_Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//�ǰݽ� ���ǵ带 ������ ���ֱ� //
	
	ZombieStat.CurrentHp -= DamageAmount;

	if (GetWorldTimerManager().IsTimerActive(SlowDelayTimer))
	{
		//������
	}
	else {
		GetWorldTimerManager().SetTimer(SlowDelayTimer, this, &ANBC_Zombie_Base_Character::MoveSpeedReset, 1.5f, false);
	}

	if (UCharacterMovementComponent* movement =
		GetCharacterMovement())
	{
		movement->MaxWalkSpeed = FMath::Clamp(movement->MaxWalkSpeed * 0.5f,
			0, 600.0f);
	}

	if (ZombieStat.CurrentHp <= 0)
	{
		Death();
	}

	return 0.0f;
}

void ANBC_Zombie_Base_Character::ZombieAttack()
{

	FVector StartLocation = GetMesh()->GetSocketLocation("RightHand");
	FVector EndLocation = StartLocation + GetActorForwardVector() * 100.0f; // 1�� 1cm����
	FHitResult HitResult;	


	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // �ڱ� �ڽ� ���� ���� �ϴ� �ڵ�

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Pawn, // ĳ���͸� �����Ѵ���.
		Params
	);

	if (bHit)
	{
		//�÷��̾� ����
		AActor* HitActor = HitResult.GetActor();
		if (HitActor &&HitActor->ActorHasTag("Player"))
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor : %s"), *HitActor->GetName());
		}
	}
}



void ANBC_Zombie_Base_Character::Death()
{
	//�׽�Ʈ��
	ZombieStat.CurrentHp = 0;

	if (ZombieStat.CurrentHp <= 0)
	{

		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName("Ragdoll");

		if (ASevenGameStateBase* test = Cast< ASevenGameStateBase>(GetWorld()->GetGameState()))
		{
			//���� �߰��� �ڵ� �־� �� ��.

		}
		
		// ���� ���� �� ������Ʈ
		ASevenGameStateBase* SevenGS = Cast<ASevenGameStateBase>(UGameplayStatics::GetGameState(this));
		if (SevenGS)
		{
			int32 NewRemainingZombies = SevenGS->GetRemainingZombies() - 1;
			SevenGS->SetRemainingZombies(NewRemainingZombies);

			if (NewRemainingZombies <= 0)
			{
				// ��� ���� ����ϸ� ���̺� ����
				ASevenGameModeBase* SevenGM = Cast<ASevenGameModeBase>(UGameplayStatics::GetGameMode(this));
				if (SevenGM)
				{
					SevenGM->EndWave();
				}
			}
		}
	}
}

void ANBC_Zombie_Base_Character::MoveSpeedReset()
{
	//�ӵ� ���󺹱�
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}




