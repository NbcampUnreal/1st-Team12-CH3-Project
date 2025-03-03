#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SevenGameStateBase.generated.h"

UCLASS()
class SEVENDAYS_API ASevenGameStateBase : public AGameStateBase
{
    GENERATED_BODY()

public:
    ASevenGameStateBase();

    /** �� ���� �� */
    UPROPERTY(BlueprintReadOnly, Category = "Zombie")
    int32 TotalZombies;

    /** ���� �����ִ� ���� �� */
    UPROPERTY(BlueprintReadOnly, Category = "Zombie")
    int32 RemainingZombies;

    UFUNCTION(BlueprintCallable, Category = "Zombie")
    int32 GetRemainingZombies() const { return RemainingZombies; }

    UFUNCTION()
    void ReduceZombieCount();


    /** ���� ���� ���� */
    void SetTotalZombies(int32 Count);
    void SetRemainingZombies(int32 Count);
    void DecreaseRemainingZombie();
};
