#include "MainMenuGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"

void AMainMenuGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    // ���� �޴� ���� �ε�
    if (TSubclassOf<UUserWidget> MainMenuClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/BP_MainMenuWidget.BP_MainMenuWidget_C")))
    {
        UMainMenuWidget* MainMenu = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuClass);
        if (MainMenu)
        {
            MainMenu->AddToViewport();
            MainMenu->ShowMainMenu();

            APlayerController* PC = GetWorld()->GetFirstPlayerController();
            if (PC)
            {
                PC->bShowMouseCursor = true;
                PC->SetInputMode(FInputModeUIOnly());
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("MainMenu widget creation failed!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load BP_MainMenuWidget!"));
    }
}
