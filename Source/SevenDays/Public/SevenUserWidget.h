#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "SevenPlayerController.h"
#include "SevenUserWidget.generated.h"


/**
 * 플레이어 HUD 위젯 클래스
 * 체력, 탄약, 무기 상태, 좀비 수, 낮밤 상태를 표시
 */
UCLASS()
class SEVENDAYS_API USevenUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;  // 위젯 초기화 함수

    /** 탄약 UI 업데이트 */
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateAmmo(int32 CurrentAmmo, int32 TotalAmmo);

    /** 무기 UI 업데이트 */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void UpdateWeaponUI(const FString& WeaponName, int32 CurrentAmmo, int32 MaxAmmo);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateWeaponIcons(EPlayerWeaponType WeaponType);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateWeaponName(const FString& WeaponName);



    /** 좀비 수 UI 업데이트 */
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateZombieCount(int32 InRemaining, int32 InTotal);

    /** 낮밤 상태 UI 업데이트 */
    UFUNCTION(BlueprintCallable, Category = "DayNight")
    void UpdateDayNightCycle(bool bIsNight);

    /** 체력 UI 업데이트 */
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHealth(float HealthPercent);

    /** 게임 오버 UI 표시 */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowGameOverUI();

protected:
    /** UI 요소가 올바르게 바인딩되었는지 확인 */
    bool EnsureWidget(UWidget* Widget, const FString& WidgetName);

    /** 체력 UI */
    UPROPERTY(meta = (BindWidget)) UProgressBar* HealthBar;
    UPROPERTY(meta = (BindWidget)) UTextBlock* HealthText;

    /** 탄약 UI */
    UPROPERTY(meta = (BindWidget)) UTextBlock* AmmoText;

    /** 무기 UI */
    /** 무기 UI */
    UPROPERTY(meta = (BindWidget)) UTextBlock* WeaponText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* WeaponNameText; 

    UPROPERTY(meta = (BindWidget)) UImage* ARImage;
    UPROPERTY(meta = (BindWidget)) UImage* PistolImage;
    UPROPERTY(meta = (BindWidget)) UImage* GrenadeImage;


    /** 낮밤 UI */
    UPROPERTY(meta = (BindWidget)) UImage* SunImage;
    UPROPERTY(meta = (BindWidget)) UImage* MoonImage;

    /** 좀비 수 UI */
    UPROPERTY(meta = (BindWidget)) UTextBlock* ZombiesText;

    /** 게임 오버 UI */
    UPROPERTY(meta = (BindWidget))
    UWidget* GameOverScreen;

    /** Quit 버튼 */
    UPROPERTY(meta = (BindWidget))
    UButton* QuitButton;

    /** Quit 버튼 클릭 이벤트 */
    UFUNCTION()
    void OnQuitButtonClicked();
};
