// Fill out your copyright notice in the Description page of Project Settings.


#include "NBC_ThrowActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ANBC_ThrowActor::ANBC_ThrowActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);

	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collision->SetCollisionResponseToAllChannels(ECR_Block); // 모든 채널에 충돌
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block); //Pawn 채널에 대해

	// 메시 컴포넌트 설정
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);


	// 충돌 이벤트 바인딩
	Collision->OnComponentHit.AddDynamic(this, &ANBC_ThrowActor::OnHit);


	// 속도 기본값
	Speed = 1000.0f;


	LaunchVelocity = Direction * Speed;

}

// Called when the game starts or when spawned
void ANBC_ThrowActor::BeginPlay()
{
	Super::BeginPlay();
	

	// 투사체의 방향을 설정 (이동 방향)
	Direction = GetActorForwardVector();

	// 이동을 시작
	SetActorLocation(GetActorLocation() + LaunchVelocity * GetWorld()->DeltaTimeSeconds);

}

// Called every frame
void ANBC_ThrowActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 투사체의 이동
	FVector NewLocation = GetActorLocation() + (Direction * Speed * DeltaTime);

	LaunchVelocity += Gravity * DeltaTime;
	NewLocation.Z += LaunchVelocity.Z * DeltaTime;

	SetActorLocation(NewLocation);
}

void ANBC_ThrowActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		//UE_LOG(LogTemp, Warning, TEXT("------- %s"), OtherActor->GetName());

		if (OtherActor->ActorHasTag("Player"))
		{
			ApplyDamage(OtherActor);
			// 충돌한 액터가 유효하면 데미지를 적용
		}

		// 충돌 후 투사체를 삭제
		Destroy();
	}
}

void ANBC_ThrowActor::ApplyDamage(AActor* HitActor)
{
	// 피해 적용 (20 데미지)
	UGameplayStatics::ApplyDamage(HitActor, 20.0f, nullptr, this, nullptr);

}

