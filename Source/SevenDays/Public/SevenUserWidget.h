#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "SevenUserWidget.generated.h"

UCLASS()
class SEVENDAYS_API USevenUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // UI 요소 바인딩
    UPROPERTY(meta = (BindWidget)) UImage* CrosshairImage;
    UPROPERTY(meta = (BindWidget)) UProgressBar* HealthBar;
    UPROPERTY(meta = (BindWidget)) UTextBlock* HealthText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* KillCountText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* AmmoText;
    UPROPERTY(meta = (BindWidget)) UImage* WeaponImage;
    UPROPERTY(meta = (BindWidget)) UTextBlock* WeaponText;
    UPROPERTY(meta = (BindWidget)) UImage* SunImage;
    UPROPERTY(meta = (BindWidget)) UImage* MoonImage;
    UPROPERTY(meta = (BindWidget)) UTextBlock* TimerText;

    // UI 업데이트 함수
    UFUNCTION(BlueprintCallable, Category = "DayNight")
    void UpdateDayNightCycle(bool bIsNight, UTexture2D* SunTexture, UTexture2D* MoonTexture);

    UFUNCTION(BlueprintCallable, Category = "Timer")
    void UpdateTimerText(int32 RemainingTime);

    void UpdateHealth(float HealthPercent);
    void UpdateKillCount(int32 KillCount);
    void UpdateAmmo(int32 CurrentAmmo, int32 TotalAmmo);
    void UpdateWeapon(FText WeaponName, UTexture2D* WeaponIcon);
};