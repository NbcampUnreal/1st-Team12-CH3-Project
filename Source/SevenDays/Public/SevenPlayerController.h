#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "SevenGameModeBase.h"
#include "SevenGameStateBase.h"
#include "SevenPlayerController.generated.h"

class USevenUserWidget;

/** 플레이어가 사용할 무기 타입 */
UENUM(BlueprintType)
enum class EPlayerWeaponType : uint8
{
    AR      UMETA(DisplayName = "Assault Rifle"),
    Pistol  UMETA(DisplayName = "Pistol"),
    Grenade UMETA(DisplayName = "Grenade")
};

/**
 * 플레이어 컨트롤러 클래스
 * - 게임 내 입력 처리, HUD UI 관리, 무기 전환, 낮/밤 전환 등을 담당합니다.
 * - 메인 메뉴와 게임 레벨에 따라 다른 동작을 하도록 설계되었습니다.
 */
UCLASS()
class SEVENDAYS_API ASevenPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupInputComponent() override;

    /** 현재 선택된 무기 타입 (기본은 Assault Rifle) */
    EPlayerWeaponType CurrentWeaponType = EPlayerWeaponType::AR;


    /** 현재 레벨이 메인 메뉴인지 여부 */
    bool bIsMainMenu = false;

public:
    ASevenPlayerController();

    /** 마우스 커서 표시 여부 설정 함수 */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetMouseVisibility(bool bVisible);

    /** 입력 매핑 컨텍스트 (Enhanced Input) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|IMC")
    UInputMappingContext* InputMappingContext;

    /** 입력 액션들 */
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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* FireAction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* ReloadAction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* ChangeARAction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* ChangeHGAction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* ChangeGLAction;

    /** 현재 게임이 밤인지 여부 */
    bool bIsNight = false;



    /** 낮/밤 전환 입력 액션 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* ToggleDayNightAction;

    /** HUD UI 위젯 클래스 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    TSubclassOf<USevenUserWidget> HUDWidgetClass;

    /** 현재 활성화된 HUD UI 위젯 */
    UPROPERTY(BlueprintReadOnly, Category = "UI")
    USevenUserWidget* CurrentWidget;

    /** 체력 UI 업데이트 함수 */
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHealth(float HealthPercent);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void UpdateWeaponUI(const FString& WeaponName, int32 CurrentAmmo, int32 MaxAmmo);

    UFUNCTION()
    void UpdateZombieUI();


    /** 게임 UI (HUD) 표시 함수 */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowGameUI();


    /** 게임 오버 UI 표시 함수 */
    void ShowGameOverScreen();
};
