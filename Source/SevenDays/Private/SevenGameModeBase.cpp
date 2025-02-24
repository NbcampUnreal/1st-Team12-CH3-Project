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

    // 기본적으로 미니게임 사용 여부를 false로 두거나,
    // 특정 상황에서 true로 바꿀 수 있음
    bUseMiniGame = false;
    bIsMiniGameActive = false;
}

void ASevenGameModeBase::StartPlay()
{
    Super::StartPlay();

    // 게임 시작 시 낮
    StartDayPhase();
    // 그리고 웨이브 시작
    StartWave();
}

// -------------------- 웨이브 로직 --------------------
void ASevenGameModeBase::StartWave()
{
    UE_LOG(LogTemp, Warning, TEXT("[Wave %d] START"), CurrentWave);

    // 낮 웨이브 시간
    RemainingTime = DayWaveDuration;

    // 타이머 리셋
    GetWorldTimerManager().ClearTimer(PhaseTimerHandle);

    // 매초 UpdateTimer() -> 시간 깎기
    GetWorldTimerManager().SetTimer(PhaseTimerHandle, this, &ASevenGameModeBase::UpdateTimer, 1.f, true);
}

void ASevenGameModeBase::EndWave()
{
    UE_LOG(LogTemp, Warning, TEXT("[Wave %d] END"), CurrentWave);
    CurrentWave++;

    // 타이머 정리
    GetWorldTimerManager().ClearTimer(PhaseTimerHandle);

    // 미니게임 선택 (bUseMiniGame가 true라면)
    if (bUseMiniGame)
    {
        bIsMiniGameActive = true;
        // 여기서 미니게임 UI/맵 열기 등 실제 로직
        // 미니게임 끝나면 OnMiniGameCompleted() 호출
    }
    else
    {
        // 미니게임이 없으면 곧바로 밤으로 넘어가거나
        // or 일정 타이머 후 밤 시작
        // 여기서는 그냥 즉시 밤 시작
        StartNightPhase();
    }
}

// -------------------- 미니게임 로직 --------------------
void ASevenGameModeBase::OnMiniGameCompleted()
{
    UE_LOG(LogTemp, Warning, TEXT("MiniGame Completed. Starting Night Phase..."));
    bIsMiniGameActive = false;

    // 미니게임이 끝나야 밤 시작
    StartNightPhase();
}

// -------------------- 낮/밤 전환 --------------------
void ASevenGameModeBase::StartDayPhase()
{
    bIsNight = false;
    UE_LOG(LogTemp, Warning, TEXT("=== Day Phase Started ==="));

    // HUD - 낮 아이콘
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
    // 필요시 사용
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

    // 밤 시간 세팅
    RemainingTime = NightDuration;

    // 타이머 리셋
    GetWorldTimerManager().ClearTimer(PhaseTimerHandle);

    // 1초마다 UpdateTimer() -> 밤 카운트다운
    GetWorldTimerManager().SetTimer(PhaseTimerHandle, this, &ASevenGameModeBase::UpdateTimer, 1.f, true);
}

void ASevenGameModeBase::EndNightPhase()
{
    UE_LOG(LogTemp, Warning, TEXT("=== Night Phase Ended ==="));

    // 타이머 정리
    GetWorldTimerManager().ClearTimer(PhaseTimerHandle);

    // 밤 끝나면 낮 + 새 웨이브
    StartDayPhase();
    StartWave();
}

// -------------------- 타이머 업데이트 --------------------
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
        // 낮 끝
        EndWave();
    }
    else
    {
        // 밤 끝
        EndNightPhase();
    }
}
