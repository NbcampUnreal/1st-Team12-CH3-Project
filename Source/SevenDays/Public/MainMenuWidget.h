#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class SEVENDAYS_API UMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UButton* StartButton;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UButton* ExitButton;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
    class UWidgetAnimation* StartButtonHoverAnim;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
    class UWidgetAnimation* ExitButtonHoverAnim;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
    class UWidgetAnimation* StartButtonUnhoverAnim;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
    class UWidgetAnimation* ExitButtonUnhoverAnim;

    UFUNCTION(BlueprintCallable)
    void OnStartClicked();

    UFUNCTION(BlueprintCallable)
    void OnExitClicked();

    UFUNCTION(BlueprintCallable)
    void OnStartHovered();

    UFUNCTION(BlueprintCallable)
    void OnStartUnhovered();

    UFUNCTION(BlueprintCallable)
    void OnExitHovered();

    UFUNCTION(BlueprintCallable)
    void OnExitUnhovered();

    UFUNCTION(BlueprintCallable)
    void LoadDayLevel();

    UFUNCTION(BlueprintCallable)
    void QuitGame();

    UFUNCTION(BlueprintCallable)
    void ShowMainMenu();

    UFUNCTION(BlueprintCallable)
    void HideMainMenu();

    UFUNCTION(BlueprintCallable)
    void ShowGameUI();

    UFUNCTION(BlueprintCallable)
    void HideGameUI();

    UFUNCTION(BlueprintCallable)
    void InitializeUI();
};
