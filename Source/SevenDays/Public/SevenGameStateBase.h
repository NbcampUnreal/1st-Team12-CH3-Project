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

    /** 총 좀비 수 */
    UPROPERTY(BlueprintReadOnly, Category = "Zombie")
    int32 TotalZombies;

    /** 현재 남아있는 좀비 수 */
    UPROPERTY(BlueprintReadOnly, Category = "Zombie")
    int32 RemainingZombies;

    UFUNCTION(BlueprintCallable, Category = "Zombie")
    int32 GetRemainingZombies() const { return RemainingZombies; }

    UFUNCTION()
    void ReduceZombieCount();


    /** 좀비 개수 설정 */
    void SetTotalZombies(int32 Count);
    void SetRemainingZombies(int32 Count);
    void DecreaseRemainingZombie();
};
