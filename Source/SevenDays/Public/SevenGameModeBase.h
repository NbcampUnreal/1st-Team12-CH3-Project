#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DayNightManager.h"
#include "SevenGameModeBase.generated.h"



UCLASS()
class SEVENDAYS_API ASevenGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASevenGameModeBase();

protected:
    virtual void StartPlay() override;

    // 현재 웨이브
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    int32 CurrentWave;

    // 낮/밤 여부
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Phase")
    bool bIsNight;

    // 낮 웨이브 시간, 밤 시간
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
    int32 DayWaveDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
    int32 NightDuration;

    // 남은 시간(초)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Phase")
    int32 RemainingTime;

    // 미니게임 사용 여부 (선택)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame")
    bool bUseMiniGame;

    // 현재 미니게임 진행 중인지
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniGame")
    bool bIsMiniGameActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight")
    ADayNightManager* DayNightManager;

    // 1초 타이머 핸들
    UPROPERTY()
    FTimerHandle PhaseTimerHandle;

protected:
    // 웨이브 시작/종료
    UFUNCTION(BlueprintCallable, Category = "Wave")
    void StartWave();
    UFUNCTION(BlueprintCallable, Category = "Wave")
    void EndWave();

    // 미니게임 로직
    UFUNCTION(BlueprintCallable, Category = "MiniGame")
    void OnMiniGameCompleted();

    // 낮/밤 전환
    void StartDayPhase();
    void EndDayPhase();
    void StartNightPhase();
    void EndNightPhase();

    private:

    // 조명 참조
   // UPROPERTY(EditAnywhere, Category = "DayNight")
   // class ADirectionalLight* DirectionalLight;



    // 1초마다 갱신
    void UpdateTimer();
    void OnPhaseTimeOver();

};
