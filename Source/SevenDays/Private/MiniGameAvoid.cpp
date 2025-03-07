#include "MiniGameAvoid.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SevenGameModeBase.h"
#include "PlayerCharacter.h"



FReply UMiniGameAvoid::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->SetShowMouseCursor(true);
    }

    FVector2D ClickPosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

    for (int32 i = 0; i < Obstacles.Num(); i++)
    {
        if (Obstacles[i])
        {
            UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Obstacles[i]);
            if (CanvasSlot)
            {
                FVector2D ObstaclePosition = CanvasSlot->GetPosition();
                FVector2D ObstacleSize = CanvasSlot->GetSize();

                FBox2D ObstacleBounds(ObstaclePosition, ObstaclePosition + ObstacleSize);
                if (ObstacleBounds.IsInside(ClickPosition))
                {
                    Obstacles[i]->RemoveFromParent();
                    Obstacles.RemoveAt(i);
                    ClickedObstacles++;
                    UpdateObstacleCount();
                    return FReply::Handled();
                }
            }
        }
    }

    return FReply::Unhandled();
}




void UMiniGameAvoid::SpawnObstacleAtRandomLocation()
{
    if (!GameCanvas || !ObstacleTemplate || SpawnedObstacles >= 70) // 50개 초과 방지
    {
        if (UWorld* World = GetWorld())
        {
            World->GetTimerManager().ClearTimer(ObstacleSpawnHandle);
        }
        return;
    }

    int32 ObstacleCount = FMath::RandRange(1, 2); // 장애물 1~2개 랜덤 생성

    for (int32 i = 0; i < ObstacleCount; i++)
    {
        if (SpawnedObstacles >= 70) break; // 50개 초과 방지

        // 안전한 포인터 사용 (WeakObjectPtr)
        TWeakObjectPtr<UImage> NewObstacle = NewObject<UImage>(this, UImage::StaticClass());
        if (!NewObstacle.IsValid()) return;

        NewObstacle->SetBrush(ObstacleTemplate->Brush);
        GameCanvas->AddChild(NewObstacle.Get());
        Obstacles.Add(NewObstacle.Get());

        FVector2D CanvasSize = GameCanvas->GetDesiredSize();
        FVector2D RandomPosition = FVector2D(
            FMath::RandRange(100, static_cast<int32>(CanvasSize.X) - 100),
            FMath::RandRange(100, static_cast<int32>(CanvasSize.Y) - 100)
        );

        UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(NewObstacle.Get());
        if (CanvasSlot)
        {
            CanvasSlot->SetPosition(RandomPosition);
            CanvasSlot->SetSize(FVector2D(50, 50));
        }

        SpawnedObstacles++;

        // 안전한 람다 캡처 사용
        FTimerHandle DestroyHandle;
        GetWorld()->GetTimerManager().SetTimer(
            DestroyHandle,
            FTimerDelegate::CreateLambda([this, NewObstacle]()
                {
                    if (NewObstacle.IsValid())  // 안전한 포인터 체크
                    {
                        Obstacles.Remove(NewObstacle.Get());
                        NewObstacle->RemoveFromParent();
                        UE_LOG(LogTemp, Log, TEXT("장애물 제거됨!"));
                    }
                }),
            1.5f,
            false
        );
    }
}



void UMiniGameAvoid::NativeConstruct()
{
    Super::NativeConstruct();

    if (StartButton)
    {
        StartButton->OnClicked.AddDynamic(this, &UMiniGameAvoid::StartCountdown);
    }
    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UMiniGameAvoid::RemoveFromParent);
        ExitButton->OnClicked.AddDynamic(this, &UMiniGameAvoid::CallMiniGameComplete);
        ExitButton->SetVisibility(ESlateVisibility::Hidden);

    }

    SetKeyboardFocus();

  //  UI 입력을 강제하도록 설정
    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->SetShowMouseCursor(true);

        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(TakeWidget()); // UI 포커스 설정
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PC->SetInputMode(InputMode);
    }
}



void UMiniGameAvoid::StartCountdown()
{
    CountdownTime = 3;
    UpdateCountdown();

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(CountdownHandle, this, &UMiniGameAvoid::UpdateCountdown, 1.0f, true);
    }
}

void UMiniGameAvoid::UpdateCountdown()
{
    if (CountdownTime <= 0)
    {
        if (UWorld* World = GetWorld())
        {
            World->GetTimerManager().ClearTimer(CountdownHandle);
        }
        StartGame();
        return;
    }

    if (CountdownText)
    {
        CountdownText->SetText(FText::AsNumber(CountdownTime));
    }
    CountdownTime--;
}

void UMiniGameAvoid::StartGame()
{
    if (CountdownText)
    {
        CountdownText->SetVisibility(ESlateVisibility::Hidden);
    }
    if (StartButton)
    {
        StartButton->SetVisibility(ESlateVisibility::Hidden);
    }

    ClickedObstacles = 0;
    SpawnedObstacles = 0;
    UpdateObstacleCount();

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(GameTimerHandle, this, &UMiniGameAvoid::EndGame, 20.0f, false); //
        World->GetTimerManager().SetTimer(ObstacleSpawnHandle, this, &UMiniGameAvoid::SpawnObstacleAtRandomLocation, 0.5f, true);
    }
}

void UMiniGameAvoid::UpdateObstacleCount()
{
    if (ObstacleCountText)
    {
        ObstacleCountText->SetText(FText::FromString(FString::Printf(TEXT("%d/50"), ClickedObstacles)));
    }
}

void UMiniGameAvoid::EndGame()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(ObstacleSpawnHandle);
        World->GetTimerManager().ClearTimer(GameTimerHandle);
    }

    // 결과 메시지 업데이트
    if (CountdownText)
    {
        FString ResultText = FString::Printf(TEXT("Get Ammo!\nClicked: %d"), ClickedObstacles);
        CountdownText->SetText(FText::FromString(ResultText));
        CountdownText->SetVisibility(ESlateVisibility::Visible);
    }

    //  Exit 버튼 표시 (미니게임 종료 후 나갈 수 있도록)
    if (ExitButton)
    {
        ExitButton->SetVisibility(ESlateVisibility::Visible);
    }

    
    UE_LOG(LogTemp, Warning, TEXT("MiniGame Finished - Showing Results Screen"));


}

void UMiniGameAvoid::CallMiniGameComplete()
{
    // 본 게임으로 나갈 수 있도록 입력 모드 복구
    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->SetInputMode(FInputModeUIOnly()); // UI 전용 입력 모드 유지
        PC->SetShowMouseCursor(true);

        // UI 유지 (제거하지 않음)
        SetVisibility(ESlateVisibility::Visible);

        // GameMode에 미니게임 종료 알림
        if (ASevenGameModeBase* GM = Cast<ASevenGameModeBase>(UGameplayStatics::GetGameMode(this)))
        {
            GM->OnMiniGameCompleted();
        }
    }

    if (APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
    {
        Player->SupplyARBullet(ClickedObstacles);
    }
}

void UMiniGameAvoid::ExitMiniGame()
{
    if (ASevenGameModeBase* GM = Cast<ASevenGameModeBase>(UGameplayStatics::GetGameMode(this)))
    {
        GM->OnMiniGameCompleted();  //  Exit 버튼을 눌렀을 때만 밤 시작!
    }

    APlayerController* PC = GetOwningPlayer();
    if (PC)
    {
        PC->SetInputMode(FInputModeGameOnly());  // 본 게임 입력 모드로 변경
        PC->SetShowMouseCursor(false);          // 마우스 숨김
    }


    RemoveFromParent();  // UI 제거
}

