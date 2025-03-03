#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (StartButton)
    {
        StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartClicked);
        StartButton->OnHovered.AddDynamic(this, &UMainMenuWidget::OnStartHovered);
        StartButton->OnUnhovered.AddDynamic(this, &UMainMenuWidget::OnStartUnhovered);
    }

    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitClicked);
        ExitButton->OnHovered.AddDynamic(this, &UMainMenuWidget::OnExitHovered);
        ExitButton->OnUnhovered.AddDynamic(this, &UMainMenuWidget::OnExitUnhovered);
    }

    InitializeUI();
}

void UMainMenuWidget::OnStartClicked()
{
    HideMainMenu();  // 메인 메뉴 UI 제거
    UGameplayStatics::OpenLevel(GetWorld(), FName("GameMap"));  // GameMap으로 이동
}


void UMainMenuWidget::OnExitClicked()
{
    QuitGame();
}

void UMainMenuWidget::QuitGame()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}

void UMainMenuWidget::OnStartHovered()
{
    if (StartButtonHoverAnim)
    {
        PlayAnimation(StartButtonHoverAnim);
    }
}

void UMainMenuWidget::OnStartUnhovered()
{
  
    if (StartButtonUnhoverAnim)
    {
        PlayAnimation(StartButtonUnhoverAnim);
    }
}

void UMainMenuWidget::OnExitHovered()
{
    if (ExitButtonHoverAnim)
    {
        PlayAnimation(ExitButtonHoverAnim);
    }
}

void UMainMenuWidget::OnExitUnhovered()
{
    if (ExitButtonUnhoverAnim)
    {
        PlayAnimation(ExitButtonUnhoverAnim);
    }
}

void UMainMenuWidget::ShowMainMenu()
{
    SetVisibility(ESlateVisibility::Visible);
}

void UMainMenuWidget::HideMainMenu()
{
    RemoveFromParent();
}

void UMainMenuWidget::ShowGameUI()
{
    if (TSubclassOf<UUserWidget> GameUIClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/BP_SevenUserWidget.BP_SevenUserWidget_C")))
    {
        UUserWidget* GameUI = CreateWidget<UUserWidget>(GetWorld(), GameUIClass);
        if (GameUI)
        {
            GameUI->AddToViewport();
            GameUI->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

void UMainMenuWidget::HideGameUI()
{
    SetVisibility(ESlateVisibility::Hidden);
}

void UMainMenuWidget::InitializeUI()
{
    HideGameUI();
    ShowMainMenu();
}

void UMainMenuWidget::LoadDayLevel()
{
    UGameplayStatics::OpenLevel(GetWorld(), FName("GameMap"));  // GameMap으로 이동
}