#include "SevenUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"
#include "Engine/Engine.h"

#include "Kismet/KismetSystemLibrary.h"


/// UI 요소 검증
bool USevenUserWidget::EnsureWidget(UWidget* Widget, const FString& WidgetName)
{
    if (!Widget)
    {
        UE_LOG(LogTemp, Error, TEXT("[SevenUserWidget] %s is NULL!"), *WidgetName);
        return false;
    }
    return true;
}


/// 위젯 초기화
void USevenUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

   // // Quit 버튼
   // if (QuitButton)
   // {
   //     QuitButton->OnClicked.AddDynamic(this, &USevenUserWidget::OnQuitButtonClicked);
   // }
   
     // 게임 오버 UI = Hidden
     if (GameOverWidget)
     {
         GameOverWidget->SetVisibility(ESlateVisibility::Hidden);
     }
   
    // UI 요소
    bool bAllWidgetsValid = true;
    if (!EnsureWidget(HealthBar, "HealthBar")) bAllWidgetsValid = false;
    if (!EnsureWidget(HealthText, "HealthText")) bAllWidgetsValid = false;
    if (!EnsureWidget(AmmoText, "AmmoText")) bAllWidgetsValid = false;
    if (!EnsureWidget(WeaponText, "WeaponText")) bAllWidgetsValid = false;
    if (!EnsureWidget(ARImage, "ARImage")) bAllWidgetsValid = false;
    if (!EnsureWidget(PistolImage, "PistolImage")) bAllWidgetsValid = false;
    if (!EnsureWidget(GrenadeImage, "GrenadeImage")) bAllWidgetsValid = false;
    if (!EnsureWidget(SunImage, "SunImage")) bAllWidgetsValid = false;
    if (!EnsureWidget(MoonImage, "MoonImage")) bAllWidgetsValid = false;
    if (!EnsureWidget(ZombiesText, "ZombiesText")) bAllWidgetsValid = false;

    if (!bAllWidgetsValid)
    {
        UE_LOG(LogTemp, Error, TEXT("[SevenUserWidget] UI NULL "));
    }

    // 기본 UI 값 설정
    ZombiesText->SetText(FText::FromString(TEXT("0 / 0")));
    //UpdateAmmo(0, 0);
    UpdateDayNightCycle(false);

}


/// 체력 UI 업데이트
void USevenUserWidget::UpdateHealth(float HealthPercent)
{
    if (!HealthBar || !HealthText) return;

    // 체력 값을 안전한 범위(0 ~ 1)로 제한
    HealthPercent = FMath::Clamp(HealthPercent, 0.0f, 1.0f);
    HealthBar->SetPercent(HealthPercent);
    HealthText->SetText(FText::AsNumber(static_cast<int32>(HealthPercent * 100)));

    UE_LOG(LogTemp, Log, TEXT("[SevenUserWidget] Health Updated: %d%%"), static_cast<int32>(HealthPercent * 100));
}


/// 무기 UI 업데이트
void USevenUserWidget::UpdateWeaponUI(const FString& WeaponName, int32 CurrentAmmo, int32 MaxAmmo)
{
    UpdateWeaponName(WeaponName);  // 무기 이름 업데이트
    UpdateAmmo(CurrentAmmo, MaxAmmo);  // 탄약 정보 업데이트

    UE_LOG(LogTemp, Warning, TEXT("Weapon UI Updated: %d / %d") , CurrentAmmo, MaxAmmo);
}


// 무기 UI 투명도 설정
void USevenUserWidget::UpdateWeaponIcons(EPlayerWeaponType WeaponType)
{
    if (!ARImage || !PistolImage || !GrenadeImage)
    {
        UE_LOG(LogTemp, Error, TEXT("[SevenUserWidget] Weapon icon images are NULL! Check UMG bindings."));
        return;
    }

    // 모든 아이콘을 기본적으로 흐리게 설정
    float InactiveOpacity = 0.2f;
    float ActiveOpacity = 1.0f;

    ARImage->SetRenderOpacity(InactiveOpacity);
    PistolImage->SetRenderOpacity(InactiveOpacity);
    GrenadeImage->SetRenderOpacity(InactiveOpacity);

    // 선택된 무기만 강조
    switch (WeaponType)
    {
    case EPlayerWeaponType::AR:
        ARImage->SetRenderOpacity(ActiveOpacity);
        break;
    case EPlayerWeaponType::Pistol:
        PistolImage->SetRenderOpacity(ActiveOpacity);
        break;
    case EPlayerWeaponType::Grenade:
        GrenadeImage->SetRenderOpacity(ActiveOpacity);
        break;
    }

    UE_LOG(LogTemp, Log, TEXT("Weapon Icon Updated: %d"), (int32)WeaponType);
}



void USevenUserWidget::UpdateWeaponName(const FString& WeaponName)
{
    if (!WeaponText)  // `WeaponText`가 NULL이면 로그 출력 후 리턴
    {
        UE_LOG(LogTemp, Error, TEXT("WeaponText is NULL! Check UMG bindings."));
        return;
    }

    WeaponText->SetText(FText::FromString(WeaponName));
}


/// 탄약 UI 업데이트
void USevenUserWidget::UpdateAmmo(int32 CurrentAmmo, int32 TotalAmmo)
{
    if (!AmmoText) return;

    AmmoText->SetText(FText::Format(FText::FromString(TEXT("{0} / {1}")),
        FText::AsNumber(CurrentAmmo),
        FText::AsNumber(TotalAmmo)));

    UE_LOG(LogTemp, Log, TEXT("[SevenUserWidget] Ammo Updated: %d / %d"), CurrentAmmo, TotalAmmo);
}


/// 좀비 수 UI 업데이트: 남은 좀비 수와 전체 좀비 수를 텍스트로 표시합니다.
void USevenUserWidget::UpdateZombieCount(int32 Remaining, int32 Total)
{
    if (!ZombiesText) return;

    ZombiesText->SetText(FText::Format(FText::FromString("{0} / {1}"), Remaining, Total));

    UE_LOG(LogTemp, Warning, TEXT("Updated Zombie Count: %d / %d"), Remaining, Total);
}


/// 낮/밤 UI 업데이트: 낮에는 해 이미지, 밤에는 달 이미지를 표시합니다.
void USevenUserWidget::UpdateDayNightCycle(bool bIsNight)
{
    if (!SunImage || !MoonImage) return;

    SunImage->SetVisibility(bIsNight ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
    MoonImage->SetVisibility(bIsNight ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}



/// ShowGameOverUI
void USevenUserWidget::ShowGameOverUI()
{
    if (GameOverWidget) return;  // 이미 UI가 있으면 생성하지 않음

    if (GameOverWidgetClass)  // 클래스가 올바르게 설정되어 있는지 확인
    {
        GameOverWidget = CreateWidget<UGameOverWidget>(GetWorld(), GameOverWidgetClass);

        if (GameOverWidget)
        {
            GameOverWidget->AddToViewport(999);
        }
    }
}

void USevenUserWidget::HitMarkUI(FVector HitLocation)
{

    APlayerController* playerController = GetOwningPlayer();

    if (HitMarkWidget && playerController)
    {
        
        // 위젯을 생성합니다.
        UUserWidget* HitMarker = CreateWidget<UUserWidget>(playerController, HitMarkWidget);
        

        if (HitMarker)
        {
            HitMarker->AddToViewport();

            // 화면 좌표로 변환 (월드 좌표 -> 화면 좌표)
            FVector2D ScreenPosition;
            playerController->ProjectWorldLocationToScreen(HitLocation, ScreenPosition);

            // UI의 위치를 설정
            HitMarker->SetPositionInViewport(ScreenPosition);
            

        }

    }
}



// /// Quit 버튼 클릭 시 호출: 게임을 종료합니다.
// void USevenUserWidget::OnQuitButtonClicked()
// {
//     UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
// }
