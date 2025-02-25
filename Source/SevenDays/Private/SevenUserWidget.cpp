#include "SevenUserWidget.h"

void USevenUserWidget::UpdateDayNightCycle(bool bIsNight, UTexture2D* SunTexture, UTexture2D* MoonTexture)
{
    if (SunImage && MoonImage)
    {
        if (SunTexture)
        {
            SunImage->SetBrushFromTexture(SunTexture);
        }
        if (MoonTexture)
        {
            MoonImage->SetBrushFromTexture(MoonTexture);
        }

        // 낮이면 해를 표시, 밤이면 달을 표시
        SunImage->SetVisibility(bIsNight ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
        MoonImage->SetVisibility(bIsNight ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

//  타이머 UI 업데이트 함수 구현
void USevenUserWidget::UpdateTimerText(int32 RemainingTime)
{
    if (TimerText)
    {
        TimerText->SetText(FText::FromString(FString::Printf(TEXT("%d"), RemainingTime)));
        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString::Printf(TEXT("UI Timer Updated: %d"), RemainingTime));
    }
}

void USevenUserWidget::UpdateHealth(float HealthPercent)
{
    if (HealthBar)
    {
        HealthBar->SetPercent(HealthPercent);
        HealthText->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), static_cast<int32>(HealthPercent * 100))));
    }
}

void USevenUserWidget::UpdateKillCount(int32 KillCount)
{
    if (KillCountText)
    {
        KillCountText->SetText(FText::FromString(FString::Printf(TEXT("Kill Count: %d"), KillCount)));
    }
}

void USevenUserWidget::UpdateAmmo(int32 CurrentAmmo, int32 TotalAmmo)
{
    if (AmmoText)
    {
        AmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentAmmo, TotalAmmo)));
    }
}

void USevenUserWidget::UpdateWeapon(FText WeaponName, UTexture2D* WeaponIcon)
{
    if (WeaponText)
    {
        WeaponText->SetText(WeaponName);
    }
    if (WeaponImage && WeaponIcon)
    {
        WeaponImage->SetBrushFromTexture(WeaponIcon);
    }
}