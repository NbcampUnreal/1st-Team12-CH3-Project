#include "SevenGameModeBase.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "SevenPlayerController.h"
#include "SevenUserWidget.h"
#include "SevenGameStateBase.h"
#include "NBC_SpawnManager.h" // 헤더 추가

ASevenGameModeBase::ASevenGameModeBase()
{
    CurrentWave = 1;
    bIsNight = false;
    DayWaveDuration = 20;
    NightDuration = 20;
    RemainingTime = 0;

    bUseMiniGame = false;
    bIsMiniGameActive = false;

    SpawnManager = nullptr;  // nullptr 초기화
}

void ASevenGameModeBase::StartPlay()
{
    Super::StartPlay();

    // DayNightManager가 설정되어 있지 않으면 스폰
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

    // SpawnManager가 설정되지 않았다면 찾기
    SpawnManager = GetWorld()->SpawnActor<ANBC_SpawnManager>();
    if (!SpawnManager)
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnManager not found!"));
    }
}

// -------------------- 웨이브 로직 --------------------
void ASevenGameModeBase::StartWave()
{
    UE_LOG(LogTemp, Warning, TEXT("[Wave %d] START"), CurrentWave);
    RemainingTime = DayWaveDuration;

    // 좀비 생성
    if (SpawnManager)
    {

        int32 SpawnCount = CurrentWave * 5; // 웨이브마다 5마리씩 증가
        FVector SpawnLocation = FVector(0.0f, 0.0f, 100.0f); // 스폰할 위치 지정
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
        // 미니게임 UI/맵 등 실제 로직 후 OnMiniGameCompleted() 호출
    }
    else
    {
        StartNightPhase();
    }
}

// -------------------- 미니게임 로직 --------------------
void ASevenGameModeBase::OnMiniGameCompleted()
{
    UE_LOG(LogTemp, Warning, TEXT("MiniGame Completed. Starting Night Phase..."));
    bIsMiniGameActive = false;
    StartNightPhase();
}

// -------------------- 낮/밤 전환 --------------------
// 중복된 StartDayPhase 중, 아래 버전으로 유지 (HUD 업데이트 및 태양 높이 설정 포함)
void ASevenGameModeBase::StartDayPhase()
{
    bIsNight = false;
    UE_LOG(LogTemp, Warning, TEXT("=== Day Phase Started ==="));

    if (DayNightManager)
    {
        // 낮 시작 시 SunHeight를 0.7로 설정한 후, 낮 지속시간 동안 -0.1로 서서히 변화
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
        // 밤 전환 시 태양 높이를 즉시 -0.1로 설정 (보간 없이)
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
    // 밤 종료 후 낮 전환 및 새 웨이브 시작
    StartDayPhase();
    StartWave();
}

// -------------------- 타이머 업데이트 --------------------
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
    // 추가적인 HUD 업데이트 로직 구현 가능
}

void ASevenGameModeBase::ToggleDayNight()
{
    // 필요 시 낮/밤 토글 기능 구현
}


