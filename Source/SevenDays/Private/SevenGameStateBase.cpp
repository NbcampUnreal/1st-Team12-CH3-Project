#include "SevenGameStateBase.h"

ASevenGameStateBase::ASevenGameStateBase()
{
    RemainingZombies = 0;
    TotalZombies = 0;
}

void ASevenGameStateBase::SetRemainingZombies(int32 NewValue)
{
    RemainingZombies = NewValue;
    UE_LOG(LogTemp, Log, TEXT( " % d"), RemainingZombies);
}

void ASevenGameStateBase::SetTotalZombies(int32 NewValue)
{
    TotalZombies = NewValue;
    UE_LOG(LogTemp, Log, TEXT(" % d"), TotalZombies);
}