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
    WheelUpAction = nullptr;
    WheelDownAction = nullptr;
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
                Subsystem->AddMappingContext(InputMappingContext, 1);
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

    // 기본 무기 설정 (Assault Rifle)
    SwitchWeapon(EPlayerWeaponType::AR);
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

void ASevenPlayerController::Move(const FInputActionValue& Value)
{
    if (APawn* ControlledPawn = GetPawn())
    {
        FVector2D MovementVector = Value.Get<FVector2D>();
        ControlledPawn->AddMovementInput(FVector::ForwardVector, MovementVector.Y);
        ControlledPawn->AddMovementInput(FVector::RightVector, MovementVector.X);
    }
}

void ASevenPlayerController::Look(const FInputActionValue& Value)
{
    FVector2D LookVector = Value.Get<FVector2D>();
    AddYawInput(LookVector.X);
    AddPitchInput(LookVector.Y);
}

void ASevenPlayerController::StartJump()
{
    if (APawn* ControlledPawn = GetPawn())
    {
        if (ACharacter* MyCharacter = Cast<ACharacter>(ControlledPawn))
        {
            MyCharacter->Jump();
        }
    }
}

void ASevenPlayerController::StopJump()
{
    if (APawn* ControlledPawn = GetPawn())
    {
        if (ACharacter* MyCharacter = Cast<ACharacter>(ControlledPawn))
        {
            MyCharacter->StopJumping();
        }
    }
}

void ASevenPlayerController::Fire(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Warning, TEXT("Fire Action Triggered"));
}


/** 입력 컴포넌트 설정 함수
 *  메인 메뉴가 아닌 경우에만 게임 내 입력 바인딩을 진행합니다.
 */
void ASevenPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();



    UE_LOG(LogTemp, Warning, TEXT("SetupInputComponent called!"));

    if (bIsMainMenu)
    {
        UE_LOG(LogTemp, Warning, TEXT("Input binding skipped in Main Menu"));
        return;
    }

    UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
    if (!EnhancedInput)
    {
        if (MoveAction) EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASevenPlayerController::Move);
        if (LookAction) EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASevenPlayerController::Look);
        if (JumpAction) EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASevenPlayerController::StartJump);
        if (FireAction) EnhancedInput->BindAction(FireAction, ETriggerEvent::Triggered, this, &ASevenPlayerController::Fire);
    }
       
    UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent Valid: Binding Start"));

    // 무기 선택 키 바인딩
    if (IA_ChangeWeapon_AR)
    {
        EnhancedInput->BindAction(IA_ChangeWeapon_AR, ETriggerEvent::Triggered, this, &ASevenPlayerController::OnSelectWeaponAR);
    }
    if (IA_ChangeWeapon_HG)
    {
        EnhancedInput->BindAction(IA_ChangeWeapon_HG, ETriggerEvent::Triggered, this, &ASevenPlayerController::OnSelectWeaponHG);
    }
    if (IA_ChangeWeapon_GL)
    {
        EnhancedInput->BindAction(IA_ChangeWeapon_GL, ETriggerEvent::Triggered, this, &ASevenPlayerController::OnSelectWeaponGL);
    }



    // 낮/밤 전환 키 바인딩
    if (ToggleDayNightAction)
    {
        EnhancedInput->BindAction(ToggleDayNightAction, ETriggerEvent::Triggered, this, &ASevenPlayerController::ToggleDayNight);
    }

    UE_LOG(LogTemp, Warning, TEXT("Input binding complete"));
}

void ASevenPlayerController::UpdateWeaponUI(const FString& WeaponName, int32 CurrentAmmo, int32 MaxAmmo)
{
    if (CurrentWidget)
    {
        CurrentWidget->UpdateWeaponUI(WeaponName, CurrentAmmo, MaxAmmo);
    }
}

/** 무기 전환 함수
 *  선택한 무기에 따라 HUD UI의 무기 정보를 업데이트합니다.
 */
void ASevenPlayerController::SwitchWeapon(EPlayerWeaponType NewWeaponType)
{
    CurrentWeaponType = NewWeaponType;

    if (CurrentWidget)
    {
        // 무기 UI 업데이트
        static const TMap<EPlayerWeaponType, FText> WeaponNames = {
            {EPlayerWeaponType::AR, FText::FromString(TEXT("Assault Rifle"))},
            {EPlayerWeaponType::Pistol, FText::FromString(TEXT("Pistol"))},
            {EPlayerWeaponType::Grenade, FText::FromString(TEXT("Grenade"))}
        };

        static const TMap<EPlayerWeaponType, int32> AmmoCounts = {
            {EPlayerWeaponType::AR, 30},
            {EPlayerWeaponType::Pistol, 12},
            {EPlayerWeaponType::Grenade, 5}
        };

        CurrentWidget->UpdateWeaponUI(
            WeaponNames[NewWeaponType].ToString(),
            AmmoCounts[NewWeaponType],
            AmmoCounts[NewWeaponType]
        );

        // ?? 선택된 무기 아이콘 업데이트
        CurrentWidget->UpdateWeaponIcons(NewWeaponType);

        UE_LOG(LogTemp, Warning, TEXT("Weapon Switched: %s"), *WeaponNames[NewWeaponType].ToString());
    }
}



/** 무기 선택 핸들러 (입력과 연결되어 호출됨) */
void ASevenPlayerController::OnSelectWeaponAR()
{
    SwitchWeapon(EPlayerWeaponType::AR);
}

void ASevenPlayerController::OnSelectWeaponHG()
{
    SwitchWeapon(EPlayerWeaponType::Pistol);
}

void ASevenPlayerController::OnSelectWeaponGL()
{
    SwitchWeapon(EPlayerWeaponType::Grenade);
}

/** 낮/밤 전환 함수
 *  게임 모드와 게임 상태를 업데이트하고, HUD UI에 변경 사항을 반영합니다.
 */
void ASevenPlayerController::ToggleDayNight()
{
    ASevenGameModeBase* GM = Cast<ASevenGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!GM) return;

    bIsNight = !bIsNight;

    if (bIsNight)
    {
        GM->TestForceNight();
    }
    else
    {
        GM->TestForceDay();
    }

    // 게임 상태에서 좀비 수를 초기화합니다.
    if (ASevenGameStateBase* GS = Cast<ASevenGameStateBase>(GetWorld()->GetGameState<ASevenGameStateBase>()))
    {
        GS->SetTotalZombies(0);
        GS->SetRemainingZombies(0);
    }

    // HUD UI에 낮/밤 전환 정보를 업데이트합니다.
    if (CurrentWidget)
    {
        CurrentWidget->UpdateDayNightCycle(bIsNight);
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


/** 낮/밤 강제 전환 (테스트용) */
void ASevenPlayerController::TestForceDay()
{
    if (ASevenGameModeBase* GM = Cast<ASevenGameModeBase>(GetWorld()->GetAuthGameMode()))
    {
        GM->TestForceDay();
    }
}

void ASevenPlayerController::TestForceNight()
{
    if (ASevenGameModeBase* GM = Cast<ASevenGameModeBase>(GetWorld()->GetAuthGameMode()))
    {
        GM->TestForceNight();
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