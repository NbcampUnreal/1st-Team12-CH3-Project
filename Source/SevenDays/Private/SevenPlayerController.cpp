
#include "SevenPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "SevenUserWidget.h"



ASevenPlayerController::ASevenPlayerController()
{
    // ���콺 Ŀ�� ���̰� �ϰ� �ʹٸ�
    bShowMouseCursor = true;
}

void ASevenPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // ������Ʈ ���� �Ǵ� �����Ϳ��� BP�� ���� HUDWidgetClass�� ������ �� ���¶�� ����
    if (HUDWidgetClass)
    {
        CurrentWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
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