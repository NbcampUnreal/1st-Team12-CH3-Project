#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/HUD.h"
#include "MiniGameAvoid.generated.h"

UCLASS()
class SEVENDAYS_API UMiniGameAvoid : public UUserWidget
{
    GENERATED_BODY()

public:

    UMiniGameAvoid(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
    {
        bIsFocusable = true;  // UI가 포커스를 받을 수 있도록 설정
    }
   
    virtual void NativeConstruct() override;
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

 
    UFUNCTION()
    void SpawnObstacleAtRandomLocation();

protected:
    UFUNCTION()
    void StartCountdown();

    UFUNCTION()
    void EndGame();

    void CallMiniGameComplete();

    void ExitMiniGame();

    void StartGame();

    void UpdateCountdown();

    UFUNCTION()
    void UpdateObstacleCount();

private:

    UPROPERTY(meta = (BindWidget))
    UImage* ObstacleTemplate;

    UPROPERTY(meta = (BindWidget))
    class UCanvasPanel* GameCanvas;

    UPROPERTY(meta = (BindWidget))
    class UButton* StartButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* ExitButton;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* CountdownText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ObstacleCountText;

    TArray<UImage*> Obstacles;
    FTimerHandle ObstacleSpawnHandle;
    FTimerHandle CountdownHandle;
    FTimerHandle GameTimerHandle;

    int32 CountdownTime;
    int32 ClickedObstacles;
    int32 SpawnedObstacles;
};
