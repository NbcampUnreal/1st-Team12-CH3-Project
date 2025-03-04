#include "SevenPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "SevenUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "SevenGameModeBase.h"
#include "SevenGameStateBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/Engine.h"

/** ������ */
ASevenPlayerController::ASevenPlayerController()
{
    InputMappingContext = nullptr;
    MoveAction = nullptr;
    LookAction = nullptr;
    JumpAction = nullptr;
    SprintAction = nullptr;
    CrouchAction = nullptr;
    FireAction = nullptr;
    ReloadAction = nullptr;
    ChangeARAction = nullptr;
    ChangeHGAction = nullptr;
    ChangeGLAction = nullptr;
}

/** ���� ���� �� ȣ��Ǵ� �Լ� */
void ASevenPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (InputMappingContext)
            {
                Subsystem->AddMappingContext(InputMappingContext, 0);
            }
        }
    }

    // ���� ���� �̸� Ȯ��
    FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld());
    UE_LOG(LogTemp, Warning, TEXT("Current Level: %s"), *CurrentLevel);

    // ���� �޴����� Ȯ��
    bIsMainMenu = (CurrentLevel == TEXT("MainMenu"));

    if (bIsMainMenu)
    {
        // ���� �޴������� ���콺 ���̱� & UI ��� ����
        SetMouseVisibility(true);
        SetInputMode(FInputModeUIOnly());

        // ���� �޴������� HUD �������� ����
        UE_LOG(LogTemp, Warning, TEXT("Main Menu Loaded - HUD NOT Created"));
        return;
    }

    // ���� ȭ�鿡���� ���콺 ����� & ���� ��� ����
    SetMouseVisibility(false);
    SetInputMode(FInputModeGameOnly());

    // HUD UI ���� (���ӿ����� ����)
    if (HUDWidgetClass)
    {
        CurrentWidget = CreateWidget<USevenUserWidget>(this, HUDWidgetClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
            CurrentWidget->SetVisibility(ESlateVisibility::Visible);
            UE_LOG(LogTemp, Warning, TEXT("Game HUD Created & Added to Viewport"));
        }
        else
        {
			UE_LOG(LogTemp, Error, TEXT("HUD Widget Creation Failed!"));
        }
    }

    // Enhanced Input �ý����� ����Ͽ� �Է� ���� �߰� (���� ����������)
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        if (InputMappingContext)
        {
            Subsystem->AddMappingContext(InputMappingContext, 1);
        }
    }

}


/** �� �����Ӹ��� ȣ��Ǵ� �Լ� */
void ASevenPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

/** ���콺 Ŀ�� ǥ�� ���θ� �����ϴ� �Լ� */
void ASevenPlayerController::SetMouseVisibility(bool bVisible)
{
    // PlayerController�� �⺻ �Լ� ����Ͽ� ���콺 Ŀ�� ǥ�� ���� ����
    SetShowMouseCursor(bVisible);

    // ���콺 Ŀ���� ���� �Է� ��� ����
    if (bVisible)
    {
        // UI ���� �Է� ���
        FInputModeUIOnly InputMode;
        SetInputMode(InputMode);
    }
    else
    {
        // ���� ���� �Է� ���
        FInputModeGameOnly InputMode;
        SetInputMode(InputMode);
    }

    UE_LOG(LogTemp, Warning, TEXT("Mouse Visibility Set: %s"), bVisible ? TEXT("Visible") : TEXT("Hidden"));
}


/** �Է� ������Ʈ ���� �Լ�
 *  ���� �޴��� �ƴ� ��쿡�� ���� �� �Է� ���ε��� �����մϴ�.
 */
void ASevenPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (bIsMainMenu)
    {
        UE_LOG(LogTemp, Warning, TEXT("Input binding skipped in Main Menu"));
        return;
    }          
}

void ASevenPlayerController::UpdateWeaponUI(const FString& WeaponName, int32 CurrentAmmo, int32 MaxAmmo)
{
    if (CurrentWidget)
    {
        CurrentWidget->UpdateWeaponUI(WeaponName, CurrentAmmo, MaxAmmo);
    }
}

void ASevenPlayerController::UpdateZombieUI()
{
    ASevenGameStateBase* GS = Cast<ASevenGameStateBase>(GetWorld()->GetGameState<ASevenGameStateBase>());
    if (GS && CurrentWidget)
    {
        CurrentWidget->UpdateZombieCount(GS->GetRemainingZombies(), GS->TotalZombies);
    }
}


/** ���� UI (HUD) ǥ�� �Լ�
 *  HUD UI�� ���̰� �ϰ�, ���� ��忡���� ���콺 Ŀ���� ����ϴ�.
 */
void ASevenPlayerController::ShowGameUI()
{
    if (bIsMainMenu)
    {
        UE_LOG(LogTemp, Warning, TEXT("ShowGameUI skipped in Main Menu"));
        return;
    }

    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
        SetMouseVisibility(false);
        UE_LOG(LogTemp, Warning, TEXT("Game Started - HUD Visible"));
    }

    //  ���⼭ �Է� ���ε� �ٽ� ����
    SetupInputComponent();
}





/** ü�� UI ������Ʈ �Լ�
 *  ü�� ������ ���� HUD UI�� ü�� ������ ������Ʈ�ϸ�, ü���� 0 ������ ��� ���� ���� ȭ���� ǥ���մϴ�.
 */
void ASevenPlayerController::UpdateHealth(float HealthPercent)
{
    if (CurrentWidget)
    {
        CurrentWidget->UpdateHealth(HealthPercent);
        UE_LOG(LogTemp, Warning, TEXT("Health Updated: %d%%"), static_cast<int32>(HealthPercent * 100));

        if (HealthPercent <= 0.0f)
        {
            ShowGameOverScreen();
        }
    }
}

/** ���� ���� ȭ�� ǥ�� �Լ� */
void ASevenPlayerController::ShowGameOverScreen()
{
    if (CurrentWidget)
    {
        CurrentWidget->ShowGameOverUI();
    }
}