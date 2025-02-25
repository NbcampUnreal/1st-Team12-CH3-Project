#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h" 
#include "sevenUserWidget.h"
#include "SevenPlayerController.generated.h"


class USevenHUDWidget;
class UInputMappingContext;
class UInputAction;

UCLASS()
class SEVENDAYS_API ASevenPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
     virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override; // Tick 함수 오버라이드


public:

    ASevenPlayerController();

	//UI
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHUD(float HealthPercent, int32 KillCount, int32 CurrentAmmo, int32 TotalAmmo, FText WeaponName, UTexture2D* WeaponIcon);

	//Character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|IMC")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* CrouchAction;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	USevenUserWidget* CurrentWidget;

protected:

	//UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<USevenUserWidget> HUDWidgetClass;

};
