// Fill out your copyright notice in the Description page of Project Settings.


#include "NBC_Zombie_Base_Character.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "NBC_SpawnManager.h"
#include "NBC_Zombie_AIController.h"
#include "SevenGameStateBase.h"


// Sets default values
ANBC_Zombie_Base_Character::ANBC_Zombie_Base_Character():
	ZombieStat(FNBC_ZombieStruct(100, 300, 10))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		HeadCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadCollision"));
		HeadCollision->SetupAttachment(MeshComp, TEXT("Head"));

		HitCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HitCollision"));

		HitCollision->OnComponentHit.AddDynamic(this, &ANBC_Zombie_Base_Character::CollisionOnHit);
	}

}

// Called when the game starts or when spawned
void ANBC_Zombie_Base_Character::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle Die;

	GetWorldTimerManager().SetTimer(Die, this, &ANBC_Zombie_Base_Character::Death, 5, false);
}

// Called every frame
void ANBC_Zombie_Base_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANBC_Zombie_Base_Character::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);

	//���������ϴ¼����ݸ��� ���ֱ�
	if (!bNewHidden)
	{
		SetActorEnableCollision(true);
	}
}

float ANBC_Zombie_Base_Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//�ǰݽ� ���ǵ带 ������ ���ֱ� //
	return 0.0f;
}

void ANBC_Zombie_Base_Character::ZombieAttack()
{
	//�̰� ������ �׽�Ʈ�� nullptr�����Ƿ� ����
	if (!HitCollision)
		return;
	//�ݸ���Ȱ��ȭ
	HitCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//���߿��׳� �������̽��� ����� ������
	HitCollision->SetWorldLocation(GetActorLocation() + FVector::ForwardVector);
}

void ANBC_Zombie_Base_Character::ZombieAttackEnd()
{
	if (!HitCollision)
		return;
	//�ݸ���Ȱ��ȭ
	HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ANBC_Zombie_Base_Character::Death()
{
	//�׽�Ʈ��
	ZombieStat.CurrentHp = 0;

	if (ZombieStat.CurrentHp <= 0)
	{
		if (!TestSpawnManager)
		{
			UE_LOG(LogTemp, Warning, TEXT("nullptr == zombie nullptr"));
		}


		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName("Ragdoll");

		//GetWorld()->GetGameState()->GetSpawnManager(this);
		if(TestSpawnManager != nullptr)
			TestSpawnManager->SetEnemy(this);

		if (ASevenGameStateBase* test = Cast< ASevenGameStateBase>(GetWorld()->GetGameState()))
		{
			//���� �߰��� �ڵ� �־� �� ��.
		}
	}
}

void ANBC_Zombie_Base_Character::CollisionOnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//���� �Ǿ����� ���̻� ���� ����
	if (!HitCollision)
		return;
	//�ݸ���Ȱ��ȭ
	HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//���� �ǰ� 
	if (OtherActor && OtherActor != this)
	{
		 //OtherActor->TakeDamage(10, )
	}

	
}


