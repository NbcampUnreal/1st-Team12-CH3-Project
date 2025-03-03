#include "SevenUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"
#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"

/// ���� �ʱ�ȭ: ������ ������ �� UI ��ҵ��� �ʱ�ȭ�ϰ� �����մϴ�.
void USevenUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Quit ��ư �̺�Ʈ ���ε� (���� ���� �ó� ���� �� ���)
    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &USevenUserWidget::OnQuitButtonClicked);
    }

    // ���� ���� UI�� �⺻������ ���� ó���մϴ�.
    if (GameOverScreen)
    {
        GameOverScreen->SetVisibility(ESlateVisibility::Hidden);
    }

    // UI ��ҵ��� �����մϴ�.
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
        UE_LOG(LogTemp, Error, TEXT("[SevenUserWidget] �Ϻ� UI ��Ұ� NULL �����Դϴ�."));
    }

    // �⺻ UI �� ����
    ZombiesText->SetText(FText::FromString(TEXT("0 / 0")));
    UpdateAmmo(0, 0);
    UpdateDayNightCycle(false);
}

/// UI ��� ���� �Լ�: Ư�� ������ ���ε��Ǿ����� Ȯ���մϴ�.
bool USevenUserWidget::EnsureWidget(UWidget* Widget, const FString& WidgetName)
{
    if (!Widget)
    {
        UE_LOG(LogTemp, Error, TEXT("[SevenUserWidget] %s is NULL!"), *WidgetName);
        return false;
    }
    return true;
}

/// ü�� UI ������Ʈ: ü�¹ٿ� ü�� �ؽ�Ʈ�� ������Ʈ�մϴ�.
void USevenUserWidget::UpdateHealth(float HealthPercent)
{
    if (!HealthBar || !HealthText) return;

    HealthPercent = FMath::Clamp(HealthPercent, 0.0f, 1.0f);
    HealthBar->SetPercent(HealthPercent);
    HealthText->SetText(FText::AsNumber(static_cast<int32>(HealthPercent * 100)));

    UE_LOG(LogTemp, Log, TEXT("[SevenUserWidget] Health Updated: %d%%"), static_cast<int32>(HealthPercent * 100));
}

/// ź�� UI ������Ʈ: ���� ź��� �� ź���� �ؽ�Ʈ�� ǥ���մϴ�.
void USevenUserWidget::UpdateAmmo(int32 CurrentAmmo, int32 TotalAmmo)
{
    if (!AmmoText) return;

    AmmoText->SetText(FText::Format(FText::FromString(TEXT("{0} / {1}")), CurrentAmmo, TotalAmmo));
    UE_LOG(LogTemp, Log, TEXT("[SevenUserWidget] Ammo Updated: %d / %d"), CurrentAmmo, TotalAmmo);
}

/// ���� UI ������Ʈ: ���� �̸��� ź��, ���õ� ���� �������� ������ ������Ʈ�մϴ�.
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

    //����������Ʈ
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

    // �⺻������ ��� �������� ������ (Alpha = 0.1)���� ����
    ARImage->SetRenderOpacity(0.5f);
    PistolImage->SetRenderOpacity(0.5f);
    GrenadeImage->SetRenderOpacity(0.5f);

    // ���õ� ���⸸ ������ (Alpha = 1.0) ����
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
    if (!WeaponText)  // `WeaponText`�� NULL�̸� �α� ��� �� ����
    {
        UE_LOG(LogTemp, Error, TEXT("WeaponText is NULL! Check UMG bindings."));
        return;
    }

    WeaponText->SetText(FText::FromString(WeaponName));
}



/// ���� �� UI ������Ʈ: ���� ���� ���� ��ü ���� ���� �ؽ�Ʈ�� ǥ���մϴ�.
void USevenUserWidget::UpdateZombieCount(int32 Remaining, int32 Total)
{
    if (!ZombiesText) return;

    ZombiesText->SetText(FText::Format(FText::FromString("{0} / {1}"), Remaining, Total));

    UE_LOG(LogTemp, Warning, TEXT("Updated Zombie Count: %d / %d"), Remaining, Total);
}


/// ��/�� UI ������Ʈ: ������ �� �̹���, �㿡�� �� �̹����� ǥ���մϴ�.
void USevenUserWidget::UpdateDayNightCycle(bool bIsNight)
{
    if (!SunImage || !MoonImage) return;

    SunImage->SetVisibility(bIsNight ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
    MoonImage->SetVisibility(bIsNight ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

/// ���� ���� UI ǥ��: ���� ���� ȭ�� ������ ���ü��� �����մϴ�.
void USevenUserWidget::ShowGameOverUI()
{
    if (GameOverScreen)
    {
        GameOverScreen->SetVisibility(ESlateVisibility::Visible);
    }
}



/// Quit ��ư Ŭ�� �� ȣ��: ������ �����մϴ�.
void USevenUserWidget::OnQuitButtonClicked()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
