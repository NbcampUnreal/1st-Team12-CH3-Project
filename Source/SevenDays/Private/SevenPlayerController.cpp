
#include "SevenPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "SevenUserWidget.h"
#include "EnhancedInputSubsystems.h"




ASevenPlayerController::ASevenPlayerController()
{
     bShowMouseCursor = true;


     //MyplayerController.cpp
     InputMappingContext = nullptr;
     MoveAction = nullptr;
     LookAction = nullptr;
     JumpAction = nullptr;
     SprintAction = nullptr;
     CrouchAction = nullptr;


}

void ASevenPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // ������Ʈ ���� �Ǵ� �����Ϳ��� BP�� ���� HUDWidgetClass�� ������ �� ���¶�� ����
    if (HUDWidgetClass)
    {
        // USevenUserWidget ���
        CurrentWidget = CreateWidget<USevenUserWidget>(this, HUDWidgetClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
        }
    }

    // �Է� ��� ����(���콺 �̿� UI ���� �����ϰ�)
    FInputModeGameAndUI InputModeData;

    if (CurrentWidget)
    {
        // TakeWidget()�� TSharedRef<SWidget>�� ��ȯ�ϹǷ� OK
        InputModeData.SetWidgetToFocus(CurrentWidget->TakeWidget());
    }
    else
    {
        // CurrentWidget�� ���� ���� ���� ��Ŀ���� ������ �����Ƿ� ȣ�� ����
        // �Ǵ� FInputModeGameOnly ���� ����� ���� ����
    }

    SetInputMode(InputModeData);


    //MyplayerController.cpp
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
}



// UI ������Ʈ �Լ� �߰�
void ASevenPlayerController::UpdateHUD (float HealthPercent, int32 KillCount, int32 CurrentAmmo, int32 TotalAmmo, FText WeaponName, UTexture2D* WeaponIcon)
{
    if (USevenUserWidget* HUD = Cast<USevenUserWidget>(CurrentWidget))
    {
        HUD->UpdateHealth(HealthPercent);
        HUD->UpdateKillCount(KillCount);
        HUD->UpdateAmmo(CurrentAmmo, TotalAmmo);
        HUD->UpdateWeapon(WeaponName, WeaponIcon);
    }
}

#include "SevenGameStateBase.h"
#include "Kismet/GameplayStatics.h"

void ASevenPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    AGameStateBase* BaseGS = UGameplayStatics::GetGameState(this);
    if (BaseGS)
    {
        ASevenGameStateBase* SevenGS = Cast<ASevenGameStateBase>(BaseGS);
        if (SevenGS && CurrentWidget)
        {
            CurrentWidget->UpdateZombieCountText(SevenGS->GetRemainingZombies(),
                SevenGS->GetTotalZombies());
        }
    }
}
