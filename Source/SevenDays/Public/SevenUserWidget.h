#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "SevenPlayerController.h"
#include "GameOverWidget.h"
#include "SevenUserWidget.generated.h"


/**
 * �÷��̾� HUD ���� Ŭ����
 * ü��, ź��, ���� ����, ���� ��, ���� ���¸� ǥ��
 */
UCLASS()
class SEVENDAYS_API USevenUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;  // ���� �ʱ�ȭ �Լ�

    /** ź�� UI ������Ʈ */
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateAmmo(int32 CurrentAmmo, int32 TotalAmmo);

    /** ���� UI ������Ʈ */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void UpdateWeaponUI(const FString& WeaponName, int32 CurrentAmmo, int32 MaxAmmo);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateWeaponIcons(EPlayerWeaponType WeaponType);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateWeaponName(const FString& WeaponName);



    /** ���� �� UI ������Ʈ */
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateZombieCount(int32 InRemaining, int32 InTotal);

    /** ���� ���� UI ������Ʈ */
    UFUNCTION(BlueprintCallable, Category = "DayNight")
    void UpdateDayNightCycle(bool bIsNight);

    /** ü�� UI ������Ʈ */
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHealth(float HealthPercent);

 

    /** ���� ���� UI ǥ�� */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowGameOverUI();

    /** ��Ʈ ��Ŀ UI ǥ�� */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void HitMarkUI(FVector HitLocation);

 
protected:
    /** UI ��Ұ� �ùٸ��� ���ε��Ǿ����� Ȯ�� */
    bool EnsureWidget(UWidget* Widget, const FString& WidgetName);

protected:

    /** ü�� UI */
    UPROPERTY(meta = (BindWidget)) UProgressBar* HealthBar;
    UPROPERTY(meta = (BindWidget)) UTextBlock* HealthText;

    /** ź�� UI */
    UPROPERTY(meta = (BindWidget)) UTextBlock* AmmoText;

    /** ���� UI */
    UPROPERTY(meta = (BindWidget)) UTextBlock* WeaponText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* WeaponNameText; 

    UPROPERTY(meta = (BindWidget)) UImage* ARImage;
    UPROPERTY(meta = (BindWidget)) UImage* PistolImage;
    UPROPERTY(meta = (BindWidget)) UImage* GrenadeImage;


    /** ���� UI */
    UPROPERTY(meta = (BindWidget)) UImage* SunImage;
    UPROPERTY(meta = (BindWidget)) UImage* MoonImage;

    /** ���� �� UI */
    UPROPERTY(meta = (BindWidget)) UTextBlock* ZombiesText;

    /** ���� ���� UI */
 //  UPROPERTY(meta = (BindWidget))
 //  UWidget* GameOverScreen;
 //
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UGameOverWidget> GameOverWidgetClass;

    UPROPERTY()
    UGameOverWidget* GameOverWidget;

    UPROPERTY()
    TSubclassOf<UUserWidget> HitMarkWidget; // ����� ����


   // /** Quit ��ư */
   // UPROPERTY(meta = (BindWidget))
   // UButton* QuitButton;
   // 
   // /** Quit ��ư Ŭ�� �̺�Ʈ */
   // UFUNCTION()
   // void OnQuitButtonClicked();
};
