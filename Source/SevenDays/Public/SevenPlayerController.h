#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h" 
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

public:

    ASevenPlayerController();

	//UI
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHUD(float HealthPercent, int32 KillCount, int32 CurrentAmmo, int32 TotalAmmo, FText WeaponName, UTexture2D* WeaponIcon);

    UPROPERTY()
    UUserWidget* CurrentWidget;


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


protected:

	//UI
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")

    TSubclassOf<UUserWidget> HUDWidgetClass;


};
