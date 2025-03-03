#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DayNightManager.h"
#include "NBC_SpawnManager.h"
#include "SevenGameModeBase.generated.h"


class NBC_Zombie_Base_Character;

/**
 * 게임 모드 클래스
 * - 웨이브 시스템 관리
 * - 낮/밤 전환
 * - 미니게임 진행
 */
UCLASS()
class SEVENDAYS_API ASevenGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASevenGameModeBase();

    /** 웨이브 시작 */
    void StartWave();

    /** 웨이브 종료 (밤 → 낮, 낮 → 밤 전환) */
    void EndWave();

    /** 전체 좀비 수 설정 */
    UFUNCTION(BlueprintCallable, Category = "Wave")
    void SetTotalZombies(int32 NewValue);

    /** 좀비 스폰 */
    void SpawnZombies();

    /** 낮으로 전환 */
    void SwitchToDay();

protected:
    virtual void BeginPlay() override;
    virtual void StartPlay() override;

    /** 낮 시작 */
    void StartDayPhase();

    /** 밤 시작 */
    void StartNightPhase();

    /** 미니게임 완료 후 밤 시작 */
    void OnMiniGameCompleted();

    /** UI 업데이트 (낮/밤 전환) */
    void UpdateDayNightUI();

    /** 낮/밤 관리 매니저 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Managers")
    ADayNightManager* DayNightManager;

    /** 좀비 스폰 매니저 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Managers")
    ANBC_SpawnManager* SpawnManager;


    /** 현재 웨이브 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    int32 CurrentWave;

    /** 현재 밤 상태 여부 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Phase")
    bool bIsNight;

    /** 미니게임 활성화 여부 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame")
    bool bUseMiniGame;

    /** 현재 미니게임 진행 여부 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniGame")
    bool bIsMiniGameActive;

    /** 웨이브 최대 개수 */
    const int32 WaveLimit = 7;

public:
    /** 낮으로 강제 변경 (테스트용) */
    UFUNCTION(BlueprintCallable, Category = "Test")
    void TestForceDay();

    /** 밤으로 강제 변경 (테스트용) */
    UFUNCTION(BlueprintCallable, Category = "Test")
    void TestForceNight();

    /** 좀비 수 강제 변경 (테스트용) */
    UFUNCTION(BlueprintCallable, Category = "Test")
    void TestSetZombieCount(int32 Remaining, int32 Total);
};
