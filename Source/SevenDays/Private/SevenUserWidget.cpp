#include "SevenUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"
#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"

/// 위젯 초기화: 위젯이 생성될 때 UI 요소들을 초기화하고 검증합니다.
void USevenUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Quit 버튼 이벤트 바인딩 (게임 오버 시나 종료 시 사용)
    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &USevenUserWidget::OnQuitButtonClicked);
    }

    // 게임 오버 UI는 기본적으로 숨김 처리합니다.
    if (GameOverScreen)
    {
        GameOverScreen->SetVisibility(ESlateVisibility::Hidden);
    }

    // UI 요소들을 검증합니다.
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
        UE_LOG(LogTemp, Error, TEXT("[SevenUserWidget] 일부 UI 요소가 NULL 상태입니다."));
    }

    // 기본 UI 값 설정
    ZombiesText->SetText(FText::FromString(TEXT("0 / 0")));
    UpdateAmmo(0, 0);
    UpdateDayNightCycle(false);
}

/// UI 요소 검증 함수: 특정 위젯이 바인딩되었는지 확인합니다.
bool USevenUserWidget::EnsureWidget(UWidget* Widget, const FString& WidgetName)
{
    if (!Widget)
    {
        UE_LOG(LogTemp, Error, TEXT("[SevenUserWidget] %s is NULL!"), *WidgetName);
        return false;
    }
    return true;
}

/// 체력 UI 업데이트: 체력바와 체력 텍스트를 업데이트합니다.
void USevenUserWidget::UpdateHealth(float HealthPercent)
{
    if (!HealthBar || !HealthText) return;

    HealthPercent = FMath::Clamp(HealthPercent, 0.0f, 1.0f);
    HealthBar->SetPercent(HealthPercent);
    HealthText->SetText(FText::AsNumber(static_cast<int32>(HealthPercent * 100)));

    UE_LOG(LogTemp, Log, TEXT("[SevenUserWidget] Health Updated: %d%%"), static_cast<int32>(HealthPercent * 100));
}

/// 탄약 UI 업데이트: 현재 탄약과 총 탄약을 텍스트로 표시합니다.
void USevenUserWidget::UpdateAmmo(int32 CurrentAmmo, int32 TotalAmmo)
{
    if (!AmmoText) return;

    AmmoText->SetText(FText::Format(FText::FromString(TEXT("{0} / {1}")), CurrentAmmo, TotalAmmo));
    UE_LOG(LogTemp, Log, TEXT("[SevenUserWidget] Ammo Updated: %d / %d"), CurrentAmmo, TotalAmmo);
}

/// 무기 UI 업데이트: 무기 이름과 탄약, 선택된 무기 아이콘의 투명도를 업데이트합니다.
void USevenUserWidget::UpdateWeaponUI(const FString& WeaponName, int32 CurrentAmmo, int32 MaxAmmo)
{
    if (!WeaponNameText || !AmmoText)
    {
        UE_LOG(LogTemp, Error, TEXT("WeaponNameText or AmmoText is NULL! Check UMG bindings."));
        return;
    }

    WeaponNameText->SetText(FText::FromString(WeaponName));
    AmmoText->SetText(FText::Format(FText::FromString("{0} / {1}"),
        FText::AsNumber(CurrentAmmo),
        FText::AsNumber(MaxAmmo)));

    //강제업데이트
   // WeaponNameText->InvalidateLayoutAndVolatility();
  //  AmmoText->InvalidateLayoutAndVolatility();

    UE_LOG(LogTemp, Warning, TEXT("Weapon UI Updated: %s, %d / %d"), *WeaponName, CurrentAmmo, MaxAmmo);
}

void USevenUserWidget::UpdateWeaponIcons(EPlayerWeaponType WeaponType)
{
    if (!ARImage || !PistolImage || !GrenadeImage)
    {
        UE_LOG(LogTemp, Error, TEXT("[SevenUserWidget] Weapon icon images are NULL! Check UMG bindings."));
        return;
    }

    // 기본적으로 모든 아이콘을 반투명 (Alpha = 0.1)으로 설정
    ARImage->SetRenderOpacity(0.5f);
    PistolImage->SetRenderOpacity(0.5f);
    GrenadeImage->SetRenderOpacity(0.5f);

    // 선택된 무기만 불투명 (Alpha = 1.0) 설정
    switch (WeaponType)
    {
    case EPlayerWeaponType::AR:
        ARImage->SetRenderOpacity(1.0f);
        break;
    case EPlayerWeaponType::Pistol:
        PistolImage->SetRenderOpacity(1.0f);
        break;
    case EPlayerWeaponType::Grenade:
        GrenadeImage->SetRenderOpacity(1.0f);
        break;
    }
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

/// 게임 오버 UI 표시: 게임 오버 화면 위젯의 가시성을 변경합니다.
void USevenUserWidget::ShowGameOverUI()
{
    if (GameOverScreen)
    {
        GameOverScreen->SetVisibility(ESlateVisibility::Visible);
    }
}



/// Quit 버튼 클릭 시 호출: 게임을 종료합니다.
void USevenUserWidget::OnQuitButtonClicked()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
