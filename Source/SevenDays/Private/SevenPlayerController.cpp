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
    WheelUpAction = nullptr;
    WheelDownAction = nullptr;
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
                Subsystem->AddMappingContext(InputMappingContext, 1);
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

    // �⺻ ���� ���� (Assault Rifle)
    SwitchWeapon(EPlayerWeaponType::AR);
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


/** �Է� ������Ʈ ���� �Լ�
 *  ���� �޴��� �ƴ� ��쿡�� ���� �� �Է� ���ε��� �����մϴ�.
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

    // ���� ���� Ű ���ε�
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



    // ��/�� ��ȯ Ű ���ε�
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

/** ���� ��ȯ �Լ�
 *  ������ ���⿡ ���� HUD UI�� ���� ������ ������Ʈ�մϴ�.
 */
void ASevenPlayerController::SwitchWeapon(EPlayerWeaponType NewWeaponType)
{
    CurrentWeaponType = NewWeaponType;

    if (CurrentWidget)
    {
        // ���� UI ������Ʈ
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

        // ?? ���õ� ���� ������ ������Ʈ
        CurrentWidget->UpdateWeaponIcons(NewWeaponType);

        UE_LOG(LogTemp, Warning, TEXT("Weapon Switched: %s"), *WeaponNames[NewWeaponType].ToString());
    }
}



/** ���� ���� �ڵ鷯 (�Է°� ����Ǿ� ȣ���) */
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

/** ��/�� ��ȯ �Լ�
 *  ���� ���� ���� ���¸� ������Ʈ�ϰ�, HUD UI�� ���� ������ �ݿ��մϴ�.
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

    // ���� ���¿��� ���� ���� �ʱ�ȭ�մϴ�.
    if (ASevenGameStateBase* GS = Cast<ASevenGameStateBase>(GetWorld()->GetGameState<ASevenGameStateBase>()))
    {
        GS->SetTotalZombies(0);
        GS->SetRemainingZombies(0);
    }

    // HUD UI�� ��/�� ��ȯ ������ ������Ʈ�մϴ�.
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


/** ��/�� ���� ��ȯ (�׽�Ʈ��) */
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