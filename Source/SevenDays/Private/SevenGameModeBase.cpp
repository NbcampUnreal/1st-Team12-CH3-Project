#include "SevenGameModeBase.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "SevenPlayerController.h"
#include "SevenUserWidget.h"
#include "SevenGameStateBase.h"
#include "NBC_SpawnManager.h" // ��� �߰�

ASevenGameModeBase::ASevenGameModeBase()
{
    CurrentWave = 1;
    bIsNight = false;
    DayWaveDuration = 20;
    NightDuration = 20;
    RemainingTime = 0;

    bUseMiniGame = false;
    bIsMiniGameActive = false;

    SpawnManager = nullptr;  // nullptr �ʱ�ȭ
}

void ASevenGameModeBase::StartPlay()
{
    Super::StartPlay();

    // DayNightManager�� �����Ǿ� ���� ������ ����
    if (!DayNightManager)
    {
        DayNightManager = GetWorld()->SpawnActor<ADayNightManager>();
        UE_LOG(LogTemp, Warning, TEXT("Spawned new DayNightManager instance!"));
    }

    if (DayNightManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("DayNightManager is now set: %s"), *DayNightManager->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to set DayNightManager!"));
    }

    StartDayPhase();
    StartWave();

    // SpawnManager�� �������� �ʾҴٸ� ã��
    SpawnManager = GetWorld()->SpawnActor<ANBC_SpawnManager>();
    if (!SpawnManager)
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnManager not found!"));
    }
}

// -------------------- ���̺� ���� --------------------
void ASevenGameModeBase::StartWave()
{
    UE_LOG(LogTemp, Warning, TEXT("[Wave %d] START"), CurrentWave);
    RemainingTime = DayWaveDuration;

    // ���� ����
    if (SpawnManager)
    {

        int32 SpawnCount = CurrentWave * 5; // ���̺긶�� 5������ ����
        FVector SpawnLocation = FVector(0.0f, 0.0f, 100.0f); // ������ ��ġ ����
        SpawnManager->CreateZombie(SpawnCount, SpawnLocation);

        ASevenGameStateBase* SevenGS = GetGameState<ASevenGameStateBase>();
        if (SevenGS)
        {
            SevenGS->SetTotalZombies(SpawnCount);
            SevenGS->SetRemainingZombies(SpawnCount);
        }
    }

    GetWorldTimerManager().ClearTimer(PhaseTimerHandle);
    GetWorldTimerManager().SetTimer(PhaseTimerHandle, this, &ASevenGameModeBase::UpdateTimer, 1.f, true);
}

void ASevenGameModeBase::EndWave()
{
    UE_LOG(LogTemp, Warning, TEXT("[Wave %d] END"), CurrentWave);
    CurrentWave++;

    GetWorldTimerManager().ClearTimer(PhaseTimerHandle);

    if (bUseMiniGame)
    {
        bIsMiniGameActive = true;
        // �̴ϰ��� UI/�� �� ���� ���� �� OnMiniGameCompleted() ȣ��
    }
    else
    {
        StartNightPhase();
    }
}

// -------------------- �̴ϰ��� ���� --------------------
void ASevenGameModeBase::OnMiniGameCompleted()
{
    UE_LOG(LogTemp, Warning, TEXT("MiniGame Completed. Starting Night Phase..."));
    bIsMiniGameActive = false;
    StartNightPhase();
}

// -------------------- ��/�� ��ȯ --------------------
// �ߺ��� StartDayPhase ��, �Ʒ� �������� ���� (HUD ������Ʈ �� �¾� ���� ���� ����)
void ASevenGameModeBase::StartDayPhase()
{
    bIsNight = false;
    UE_LOG(LogTemp, Warning, TEXT("=== Day Phase Started ==="));

    if (DayNightManager)
    {
        // �� ���� �� SunHeight�� 0.7�� ������ ��, �� ���ӽð� ���� -0.1�� ������ ��ȭ
        DayNightManager->SetSunHeight(0.7f);
        DayNightManager->SetDayNightState(EDayNightState::Day, DayWaveDuration);
    }

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
    if (DayNightManager)
    {
        DayNightManager->SetSunHeight(-1.0f);
    }
}

void ASevenGameModeBase::StartNightPhase()
{
    bIsNight = true;
    UE_LOG(LogTemp, Warning, TEXT("=== Night Phase Started ==="));

    if (DayNightManager)
    {
        // �� ��ȯ �� �¾� ���̸� ��� -0.1�� ���� (���� ����)
        DayNightManager->SetDayNightState(EDayNightState::Night, 0);
    }

    RemainingTime = NightDuration;

    GetWorldTimerManager().ClearTimer(PhaseTimerHandle);
    GetWorldTimerManager().SetTimer(PhaseTimerHandle, this, &ASevenGameModeBase::UpdateTimer, 1.f, true);

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

void ASevenGameModeBase::EndNightPhase()
{
    UE_LOG(LogTemp, Warning, TEXT("=== Night Phase Ended ==="));

    GetWorldTimerManager().ClearTimer(PhaseTimerHandle);
    // �� ���� �� �� ��ȯ �� �� ���̺� ����
    StartDayPhase();
    StartWave();
}

// -------------------- Ÿ�̸� ������Ʈ --------------------
void ASevenGameModeBase::UpdateTimer()
{
    if (RemainingTime > 0)
    {
        RemainingTime--;

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
        EndWave();
    }
    else
    {
        EndNightPhase();
    }
}

void ASevenGameModeBase::UpdateHUD()
{
    // �߰����� HUD ������Ʈ ���� ���� ����
}

void ASevenGameModeBase::ToggleDayNight()
{
    // �ʿ� �� ��/�� ��� ��� ����
}


