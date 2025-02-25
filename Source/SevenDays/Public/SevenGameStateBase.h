#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SevenGameStateBase.generated.h"

UCLASS()
class SEVENDAYS_API ASevenGameStateBase : public AGameStateBase
{
    GENERATED_BODY()

public:
    ASevenGameStateBase();

protected:
    // ���� ���� ��
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Wave")
    int32 RemainingZombies;

    // �� ����
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Score")
    int32 TotalScore;

public:
    // ���� ���� �� ����
    UFUNCTION(BlueprintCallable, Category = "Wave")
    void SetRemainingZombies(int32 NewValue);

    // ���� ���� �� ��ȯ
    UFUNCTION(BlueprintCallable, Category = "Wave")
    int32 GetRemainingZombies() const { return RemainingZombies; }

    // ���� �߰�
    UFUNCTION(BlueprintCallable, Category = "Score")
    void AddScore(int32 ScoreToAdd);

    // �� ���� ��ȯ
    UFUNCTION(BlueprintCallable, Category = "Score")
    int32 GetTotalScore() const { return TotalScore; }


};
