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

protected:
    // 남은 좀비 수
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Wave")
    int32 RemainingZombies;

    // 전체 좀비 수 (Wave 시작 시 스폰된 좀비 수)
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Wave")
    int32 TotalZombies;

public:
    // 남은 좀비 수 설정
    UFUNCTION(BlueprintCallable, Category = "Wave")
    void SetRemainingZombies(int32 NewValue);

    UFUNCTION(BlueprintCallable, Category = "Wave")
    int32 GetRemainingZombies() const { return RemainingZombies; }

    // 전체 좀비 수 설정
    UFUNCTION(BlueprintCallable, Category = "Wave")
    void SetTotalZombies(int32 NewValue);

    UFUNCTION(BlueprintCallable, Category = "Wave")
    int32 GetTotalZombies() const { return TotalZombies; }
};
