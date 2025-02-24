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

    // ���� ���̺�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    int32 CurrentWave;

    // ��/�� ����
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Phase")
    bool bIsNight;

    // �� ���̺� �ð�, �� �ð�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
    int32 DayWaveDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
    int32 NightDuration;

    // ���� �ð�(��)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Phase")
    int32 RemainingTime;

    // �̴ϰ��� ��� ���� (����)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame")
    bool bUseMiniGame;

    // ���� �̴ϰ��� ���� ������
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniGame")
    bool bIsMiniGameActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight")
    ADayNightManager* DayNightManager;

    // 1�� Ÿ�̸� �ڵ�
    UPROPERTY()
    FTimerHandle PhaseTimerHandle;

protected:
    // ���̺� ����/����
    UFUNCTION(BlueprintCallable, Category = "Wave")
    void StartWave();
    UFUNCTION(BlueprintCallable, Category = "Wave")
    void EndWave();

    // �̴ϰ��� ����
    UFUNCTION(BlueprintCallable, Category = "MiniGame")
    void OnMiniGameCompleted();

    // ��/�� ��ȯ
    void StartDayPhase();
    void EndDayPhase();
    void StartNightPhase();
    void EndNightPhase();

    private:

    // ���� ����
   // UPROPERTY(EditAnywhere, Category = "DayNight")
   // class ADirectionalLight* DirectionalLight;



    // 1�ʸ��� ����
    void UpdateTimer();
    void OnPhaseTimeOver();

};
