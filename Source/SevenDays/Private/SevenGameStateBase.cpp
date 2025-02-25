#include "SevenGameStateBase.h"

ASevenGameStateBase::ASevenGameStateBase()
{
    RemainingZombies = 0;
    TotalScore = 0;
}

void ASevenGameStateBase::SetRemainingZombies(int32 NewValue)
{
    RemainingZombies = NewValue;
    UE_LOG(LogTemp, Log, TEXT("���� ���� ��: %d"), RemainingZombies);
}

void ASevenGameStateBase::AddScore(int32 ScoreToAdd)
{
    TotalScore += ScoreToAdd;
    UE_LOG(LogTemp, Log, TEXT("���� ����: %d"), TotalScore);
}