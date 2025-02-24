#include "SevenGameModeBase.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "SevenPlayerController.h"
#include "SevenUserWidget.h"

ASevenGameModeBase::ASevenGameModeBase()
{
    CurrentWave = 1;
    bIsNight = false;
    DayWaveDuration = 20;
    NightDuration = 20;
    RemainingTime = 0;

    // �⺻������ �̴ϰ��� ��� ���θ� false�� �ΰų�,
    // Ư�� ��Ȳ���� true�� �ٲ� �� ����
    bUseMiniGame = false;
    bIsMiniGameActive = false;
}

void ASevenGameModeBase::StartPlay()
{
    Super::StartPlay();

    // ���� ���� �� ��
    StartDayPhase();
    // �׸��� ���̺� ����
    StartWave();
}

// -------------------- ���̺� ���� --------------------
void ASevenGameModeBase::StartWave()
{
    UE_LOG(LogTemp, Warning, TEXT("[Wave %d] START"), CurrentWave);

    // �� ���̺� �ð�
    RemainingTime = DayWaveDuration;

    // Ÿ�̸� ����
    GetWorldTimerManager().ClearTimer(PhaseTimerHandle);

    // ���� UpdateTimer() -> �ð� ���
    GetWorldTimerManager().SetTimer(PhaseTimerHandle, this, &ASevenGameModeBase::UpdateTimer, 1.f, true);
}

void ASevenGameModeBase::EndWave()
{
    UE_LOG(LogTemp, Warning, TEXT("[Wave %d] END"), CurrentWave);
    CurrentWave++;

    // Ÿ�̸� ����
    GetWorldTimerManager().ClearTimer(PhaseTimerHandle);

    // �̴ϰ��� ���� (bUseMiniGame�� true���)
    if (bUseMiniGame)
    {
        bIsMiniGameActive = true;
        // ���⼭ �̴ϰ��� UI/�� ���� �� ���� ����
        // �̴ϰ��� ������ OnMiniGameCompleted() ȣ��
    }
    else
    {
        // �̴ϰ����� ������ ��ٷ� ������ �Ѿ�ų�
        // or ���� Ÿ�̸� �� �� ����
        // ���⼭�� �׳� ��� �� ����
        StartNightPhase();
    }
}

// -------------------- �̴ϰ��� ���� --------------------
void ASevenGameModeBase::OnMiniGameCompleted()
{
    UE_LOG(LogTemp, Warning, TEXT("MiniGame Completed. Starting Night Phase..."));
    bIsMiniGameActive = false;

    // �̴ϰ����� ������ �� ����
    StartNightPhase();
}

// -------------------- ��/�� ��ȯ --------------------
void ASevenGameModeBase::StartDayPhase()
{
    bIsNight = false;
    UE_LOG(LogTemp, Warning, TEXT("=== Day Phase Started ==="));

    // HUD - �� ������
    if (ASevenPlayerController* PC = Cast<ASevenPlayerController>(GetWorld()->GetFirstPlayerController()))
    {
        if (PC->CurrentWidget)
        {
            if (USevenUserWidget* HUD = Cast<USevenUserWidget>(PC->CurrentWidget))
            {
                HUD->UpdateDayNightCycle(bIsNight, nullptr, nullptr);
            }
        }
    }
}

void ASevenGameModeBase::EndDayPhase()
{
    UE_LOG(LogTemp, Warning, TEXT("=== Day Phase Ended ==="));
    // �ʿ�� ���
}

void ASevenGameModeBase::StartNightPhase()
{
    bIsNight = true;
    UE_LOG(LogTemp, Warning, TEXT("=== Night Phase Started ==="));

    if (ASevenPlayerController* PC = Cast<ASevenPlayerController>(GetWorld()->GetFirstPlayerController()))
    {
        if (PC->CurrentWidget)
        {
            if (USevenUserWidget* HUD = Cast<USevenUserWidget>(PC->CurrentWidget))
            {
                HUD->UpdateDayNightCycle(bIsNight, nullptr, nullptr);
            }
        }
    }

    // �� �ð� ����
    RemainingTime = NightDuration;

    // Ÿ�̸� ����
    GetWorldTimerManager().ClearTimer(PhaseTimerHandle);

    // 1�ʸ��� UpdateTimer() -> �� ī��Ʈ�ٿ�
    GetWorldTimerManager().SetTimer(PhaseTimerHandle, this, &ASevenGameModeBase::UpdateTimer, 1.f, true);
}

void ASevenGameModeBase::EndNightPhase()
{
    UE_LOG(LogTemp, Warning, TEXT("=== Night Phase Ended ==="));

    // Ÿ�̸� ����
    GetWorldTimerManager().ClearTimer(PhaseTimerHandle);

    // �� ������ �� + �� ���̺�
    StartDayPhase();
    StartWave();
}

// -------------------- Ÿ�̸� ������Ʈ --------------------
void ASevenGameModeBase::UpdateTimer()
{
    if (RemainingTime > 0)
    {
        RemainingTime--;

        // HUD
        if (ASevenPlayerController* PC = Cast<ASevenPlayerController>(GetWorld()->GetFirstPlayerController()))
        {
            if (PC->CurrentWidget)
            {
                if (USevenUserWidget* HUD = Cast<USevenUserWidget>(PC->CurrentWidget))
                {
                    HUD->UpdateTimerText(RemainingTime);
                }
            }
        }

        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("RemainingTime: %d"), RemainingTime));
    }
    else
    {
        OnPhaseTimeOver();
    }
}

void ASevenGameModeBase::OnPhaseTimeOver()
{
    GetWorldTimerManager().ClearTimer(PhaseTimerHandle);

    if (!bIsNight)
    {
        // �� ��
        EndWave();
    }
    else
    {
        // �� ��
        EndNightPhase();
    }
}
