#include "SevenGameStateBase.h"
#include "SevenPlayerController.h"
#include "Kismet/GameplayStatics.h"

ASevenGameStateBase::ASevenGameStateBase()
{
    TotalZombies = 0;
    RemainingZombies = 0;
}

void ASevenGameStateBase::SetTotalZombies(int32 Count)
{
    TotalZombies = Count;
    RemainingZombies = Count;
}

void ASevenGameStateBase::SetRemainingZombies(int32 Count)
{
    RemainingZombies = Count;
}

void ASevenGameStateBase::DecreaseRemainingZombie()
{
    if (RemainingZombies > 0)
    {
        RemainingZombies--;
    }
}

void ASevenGameStateBase::ReduceZombieCount()
{
    if (RemainingZombies > 0)
    {
        RemainingZombies--;
        UE_LOG(LogTemp, Warning, TEXT("Zombie Count Updated: %d"), RemainingZombies);
    }

    // UI 업데이트
    ASevenPlayerController* PC = Cast<ASevenPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (PC)
    {
        PC->UpdateZombieUI();
    }

    // 모든 좀비가 죽었으면 낮으로 전환
    if (RemainingZombies == 0)
    {
        ASevenGameModeBase* SevenGM = Cast<ASevenGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
        if (SevenGM)
        {
            SevenGM->EndWave();
        }
    }
}

