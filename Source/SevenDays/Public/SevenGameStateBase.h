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

    // ��ü ���� �� (Wave ���� �� ������ ���� ��)
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Wave")
    int32 TotalZombies;

public:
    // ���� ���� �� ����
    UFUNCTION(BlueprintCallable, Category = "Wave")
    void SetRemainingZombies(int32 NewValue);

    UFUNCTION(BlueprintCallable, Category = "Wave")
    int32 GetRemainingZombies() const { return RemainingZombies; }

    // ��ü ���� �� ����
    UFUNCTION(BlueprintCallable, Category = "Wave")
    void SetTotalZombies(int32 NewValue);

    UFUNCTION(BlueprintCallable, Category = "Wave")
    int32 GetTotalZombies() const { return TotalZombies; }
};
