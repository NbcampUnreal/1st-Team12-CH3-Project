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

    // 총 점수
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Score")
    int32 TotalScore;

public:
    // 남은 좀비 수 설정
    UFUNCTION(BlueprintCallable, Category = "Wave")
    void SetRemainingZombies(int32 NewValue);

    // 남은 좀비 수 반환
    UFUNCTION(BlueprintCallable, Category = "Wave")
    int32 GetRemainingZombies() const { return RemainingZombies; }

    // 점수 추가
    UFUNCTION(BlueprintCallable, Category = "Score")
    void AddScore(int32 ScoreToAdd);

    // 총 점수 반환
    UFUNCTION(BlueprintCallable, Category = "Score")
    int32 GetTotalScore() const { return TotalScore; }


};
