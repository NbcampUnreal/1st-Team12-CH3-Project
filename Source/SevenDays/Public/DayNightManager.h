#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DayNightManager.generated.h"

UENUM(BlueprintType)
enum class EDayNightState : uint8
{
    Day UMETA(DisplayName = "Day"),
    Night UMETA(DisplayName = "Night")
};

UCLASS()
class SEVENDAYS_API ADayNightManager : public AActor
{
    GENERATED_BODY()

public:
    ADayNightManager();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DayNight")
    EDayNightState CurrentState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    class ADirectionalLight* DirectionalLightActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    class ASkyLight* SkyLightActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky")
    AActor* SkySphere;

    // 낮/밤 상태
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight")
    bool bIsDaytime;

    UFUNCTION(BlueprintCallable, Category = "DayNight")
    void SetSunHeight(float NewSunHeight);


    UFUNCTION(BlueprintCallable, Category = "DayNight")
    void SetDayNightState(EDayNightState NewState, float DayDuration);

    void SmoothTransitionSunHeight();    
    
    
    void UpdateLighting();
    void UpdateSkySphere();
    // 낮/밤에 따른 SunHeight 업데이트
    void UpdateSunHeight();
    // 추가 멤버 변수: 전환에 걸리는 시간 추적용
    float TransitionElapsedTime;
    float TransitionTotalTime;

private:



    float CurrentSunHeight;
    float TargetSunHeight;
    float SunHeightInterpSpeed;

    FTimerHandle SmoothTransitionHandle;
};
