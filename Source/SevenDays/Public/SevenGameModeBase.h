#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DayNightManager.h"
#include "NBC_SpawnManager.h"
#include "SevenGameModeBase.generated.h"


class NBC_Zombie_Base_Character;

/**
 * ���� ��� Ŭ����
 * - ���̺� �ý��� ����
 * - ��/�� ��ȯ
 * - �̴ϰ��� ����
 */
UCLASS()
class SEVENDAYS_API ASevenGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASevenGameModeBase();

    /** ���̺� ���� */
    void StartWave();

    /** ���̺� ���� (�� �� ��, �� �� �� ��ȯ) */
    void EndWave();

    /** ��ü ���� �� ���� */
    UFUNCTION(BlueprintCallable, Category = "Wave")
    void SetTotalZombies(int32 NewValue);

    /** ���� ���� */
    void SpawnZombies();

    /** ������ ��ȯ */
    void SwitchToDay();

protected:
    virtual void BeginPlay() override;
    virtual void StartPlay() override;

    /** �� ���� */
    void StartDayPhase();

    /** �� ���� */
    void StartNightPhase();

    /** �̴ϰ��� �Ϸ� �� �� ���� */
    void OnMiniGameCompleted();

    /** UI ������Ʈ (��/�� ��ȯ) */
    void UpdateDayNightUI();

    /** ��/�� ���� �Ŵ��� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Managers")
    ADayNightManager* DayNightManager;

    /** ���� ���� �Ŵ��� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Managers")
    ANBC_SpawnManager* SpawnManager;


    /** ���� ���̺� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    int32 CurrentWave;

    /** ���� �� ���� ���� */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Phase")
    bool bIsNight;

    /** �̴ϰ��� Ȱ��ȭ ���� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame")
    bool bUseMiniGame;

    /** ���� �̴ϰ��� ���� ���� */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniGame")
    bool bIsMiniGameActive;

    /** ���̺� �ִ� ���� */
    const int32 WaveLimit = 7;

public:
    /** ������ ���� ���� (�׽�Ʈ��) */
    UFUNCTION(BlueprintCallable, Category = "Test")
    void TestForceDay();

    /** ������ ���� ���� (�׽�Ʈ��) */
    UFUNCTION(BlueprintCallable, Category = "Test")
    void TestForceNight();

    /** ���� �� ���� ���� (�׽�Ʈ��) */
    UFUNCTION(BlueprintCallable, Category = "Test")
    void TestSetZombieCount(int32 Remaining, int32 Total);
};
