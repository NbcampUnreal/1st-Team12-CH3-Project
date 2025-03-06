// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NBC_ThrowActor.generated.h"

class USphereComponent;

UCLASS()
class SEVENDAYS_API ANBC_ThrowActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANBC_ThrowActor();

	FVector Direction;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 포물선 발사를 위한 속도 설정
	FVector LaunchVelocity = Direction * Speed;
	FVector Gravity = FVector(0, 0, -980.f); // 중력 (Z축 방향으로)
	
	// 투사체 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value")
	float Speed;

	//컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USphereComponent* Collision; //  타격 범위

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* MeshComponent;

	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	// 데미지를 주는 함수
	void ApplyDamage(AActor* HitActor);

};
