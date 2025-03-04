#include "SevenPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "SevenUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "SevenGameModeBase.h"
#include "SevenGameStateBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/Engine.h"

/** 생성자 */
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

/** 게임 시작 시 호출되는 함수 */
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

    // 현재 레벨 이름 확인
    FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld());
    UE_LOG(LogTemp, Warning, TEXT("Current Level: %s"), *CurrentLevel);

    // 메인 메뉴인지 확인
    bIsMainMenu = (CurrentLevel == TEXT("MainMenu"));

    if (bIsMainMenu)
    {
        // 메인 메뉴에서는 마우스 보이기 & UI 모드 적용
        SetMouseVisibility(true);
        SetInputMode(FInputModeUIOnly());

        // 메인 메뉴에서는 HUD 생성하지 않음
        UE_LOG(LogTemp, Warning, TEXT("Main Menu Loaded - HUD NOT Created"));
        return;
    }

    // 게임 화면에서는 마우스 숨기기 & 게임 모드 적용
    SetMouseVisibility(false);
    SetInputMode(FInputModeGameOnly());

    // HUD UI 생성 (게임에서만 생성)
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

    // Enhanced Input 시스템을 사용하여 입력 매핑 추가 (게임 레벨에서만)
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        if (InputMappingContext)
        {
            Subsystem->AddMappingContext(InputMappingContext, 1);
        }
    }

}


/** 매 프레임마다 호출되는 함수 */
void ASevenPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

/** 마우스 커서 표시 여부를 설정하는 함수 */
void ASevenPlayerController::SetMouseVisibility(bool bVisible)
{
    // PlayerController의 기본 함수 사용하여 마우스 커서 표시 여부 설정
    SetShowMouseCursor(bVisible);

    // 마우스 커서에 따른 입력 모드 설정
    if (bVisible)
    {
        // UI 전용 입력 모드
        FInputModeUIOnly InputMode;
        SetInputMode(InputMode);
    }
    else
    {
        // 게임 전용 입력 모드
        FInputModeGameOnly InputMode;
        SetInputMode(InputMode);
    }

    UE_LOG(LogTemp, Warning, TEXT("Mouse Visibility Set: %s"), bVisible ? TEXT("Visible") : TEXT("Hidden"));
}


/** 입력 컴포넌트 설정 함수
 *  메인 메뉴가 아닌 경우에만 게임 내 입력 바인딩을 진행합니다.
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


/** 게임 UI (HUD) 표시 함수
 *  HUD UI를 보이게 하고, 게임 모드에서는 마우스 커서를 숨깁니다.
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

    //  여기서 입력 바인딩 다시 실행
    SetupInputComponent();
}





/** 체력 UI 업데이트 함수
 *  체력 비율에 따라 HUD UI의 체력 정보를 업데이트하며, 체력이 0 이하일 경우 게임 오버 화면을 표시합니다.
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

/** 게임 오버 화면 표시 함수 */
void ASevenPlayerController::ShowGameOverScreen()
{
    if (CurrentWidget)
    {
        CurrentWidget->ShowGameOverUI();
    }
}