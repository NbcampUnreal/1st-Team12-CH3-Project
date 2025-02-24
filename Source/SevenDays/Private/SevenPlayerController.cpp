
#include "SevenPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "SevenUserWidget.h"



ASevenPlayerController::ASevenPlayerController()
{
    // 마우스 커서 보이게 하고 싶다면
    bShowMouseCursor = true;
}

void ASevenPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 프로젝트 세팅 또는 에디터에서 BP로 만든 HUDWidgetClass를 지정해 둔 상태라고 가정
    if (HUDWidgetClass)
    {
        CurrentWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
        }

    }

    // 입력 모드 설정(마우스 이용 UI 조작 가능하게)
    FInputModeGameAndUI InputModeData;

    if (CurrentWidget)
    {
        // TakeWidget()이 TSharedRef<SWidget>를 반환하므로 OK
        InputModeData.SetWidgetToFocus(CurrentWidget->TakeWidget());
    }
    else
    {
        // CurrentWidget이 없을 때는 굳이 포커스할 위젯이 없으므로 호출 생략
        // 또는 FInputModeGameOnly 등을 사용할 수도 있음
    }

    SetInputMode(InputModeData);

}

// UI 업데이트 함수 추가
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