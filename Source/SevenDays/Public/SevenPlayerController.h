#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h" // UUserWidget Æ÷ÇÔ
#include "SevenPlayerController.generated.h"

class USevenHUDWidget;

UCLASS()
class SEVENDAYS_API ASevenPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

public:

    ASevenPlayerController();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHUD(float HealthPercent, int32 KillCount, int32 CurrentAmmo, int32 TotalAmmo, FText WeaponName, UTexture2D* WeaponIcon);

    UPROPERTY()
    UUserWidget* CurrentWidget;

protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")

    TSubclassOf<UUserWidget> HUDWidgetClass;


};
