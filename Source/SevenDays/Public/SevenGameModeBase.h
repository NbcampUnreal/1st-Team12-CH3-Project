#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DayNightManager.h"
#include "NBC_SpawnManager.h"
#include "SevenGameModeBase.generated.h"



UCLASS()
class SEVENDAYS_API ASevenGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASevenGameModeBase();

    // 웨이브 시작/종료
    UFUNCTION(BlueprintCallable, Category = "Wave")
    void StartWave();
    UFUNCTION(BlueprintCallable, Category = "Wave")
    void EndWave();

protected:


    virtual void StartPlay() override;

    // 스폰 매니저 추가
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    ANBC_SpawnManager* SpawnManager; 

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

    // 낮/밤 전환을 관리하는 매니저
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight")
    ADayNightManager* DayNightManager;

    // 1초 타이머 핸들
    UPROPERTY()
    FTimerHandle PhaseTimerHandle;



protected:


    // 미니게임 로직
    UFUNCTION(BlueprintCallable, Category = "MiniGame")
    void OnMiniGameCompleted();

    // 낮/밤 전환
    void StartDayPhase();
    void EndDayPhase();
    void StartNightPhase();
    void EndNightPhase();

    // 1초마다 갱신
    void UpdateTimer();
    void OnPhaseTimeOver();

private:
    // HUD 업데이트 (시간 및 낮/밤 표시)
    void UpdateHUD();

    void ToggleDayNight();
};
