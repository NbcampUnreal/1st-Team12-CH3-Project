#include "SevenGameModeBase.h"
#include "PlayerCharacter.h"
#include "DayNightManager.h"
#include "NBC_SpawnManager.h"
#include "SevenPlayerController.h"
#include "SevenUserWidget.h"
#include "SevenGameStateBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


/** 생성자: 기본값 설정 */
ASevenGameModeBase::ASevenGameModeBase()
{
    DefaultPawnClass = APlayerCharacter::StaticClass();

    CurrentWave = 1;
    bIsNight = false;
    bUseMiniGame = false;
    bIsMiniGameActive = false;
}

/** 게임 시작 시 실행 (UI 설정 포함) */
void ASevenGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    RestartPlayer(GetWorld()->GetFirstPlayerController());  // PlayerStart에서 스폰

    // 게임 맵 감지 후 HUD 추가
    if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == TEXT("GameMap"))
    {
        UE_LOG(LogTemp, Warning, TEXT("[SevenGameModeBase] GameMap detected, setting up UI."));

        TSubclassOf<UUserWidget> SevenHUDClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/BP_SevenUserWidget.BP_SevenUserWidget_C"));
        if (SevenHUDClass)
        {
            UUserWidget* SevenHUD = CreateWidget<UUserWidget>(GetWorld(), SevenHUDClass);
            if (SevenHUD)
            {
                SevenHUD->AddToViewport();
            }
        }
    }


     FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld());
    
     if (CurrentLevel == TEXT("GameMap"))
     {
         UE_LOG(LogTemp, Warning, TEXT("GameMap Loaded - Starting MiniGame"));
    
         // 미니게임 자동 실행
         StartMiniGame();
     }

     
 //  "킬 확정 UI" 블루프린트 로드 (중복 제거)
     KillConfirmUIClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/WB_KillConfirm.WB_KillConfirm_C"));
     if (!KillConfirmUIClass)
     {
         UE_LOG(LogTemp, Error, TEXT("Failed to load Kill Confirm UI. Check the Blueprint path!"));
         return;
     }

     APlayerController* PC = GetWorld()->GetFirstPlayerController();
     if (!PC) return;

     //  "킬 확정 UI" 생성
     KillConfirmUI = CreateWidget<UUserWidget>(PC, KillConfirmUIClass);
     if (KillConfirmUI)
     {
         KillConfirmUI->AddToViewport();
         KillConfirmUI->SetVisibility(ESlateVisibility::Hidden);  // 기본적으로 숨김
         UE_LOG(LogTemp, Warning, TEXT("Kill Confirm UI Loaded Successfully!"));
     }

}

void ASevenGameModeBase::OnEnemyKilled()
{
    if (!KillConfirmUI) return; // UI가 없으면 실행하지 않음.

    KillConfirmUI->SetVisibility(ESlateVisibility::Visible);  // UI 표시

    // 1.5초 후 UI 숨기기
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, [this]()
        {
            if (KillConfirmUI)
            {
                KillConfirmUI->SetVisibility(ESlateVisibility::Hidden);
            }
        }, 1.5f, false);

    UE_LOG(LogTemp, Warning, TEXT("Kill Confirm UI Displayed!"));
}




/** 게임 시작 시 호출 (매니저 초기화 포함) */
void ASevenGameModeBase::StartPlay()
{
    Super::StartPlay();

    // DayNightManager & SpawnManager 찾기
    DayNightManager = Cast<ADayNightManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADayNightManager::StaticClass()));
    SpawnManager = Cast<ANBC_SpawnManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ANBC_SpawnManager::StaticClass()));

    // 낮으로 시작 & 첫 웨이브 시작
    StartDayPhase();
    StartWave();
}



/** 웨이브 시작 */
void ASevenGameModeBase::StartWave()
{
    // 밤이 아니면 좀비 스폰 X
    if (!bIsNight)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Wave %d] Skipped - Night Phase Only"), CurrentWave);
        return;
    }

    // 좀비 스폰 실행
    if (SpawnManager)
    {
        int32 SpawnCount = FMath::Clamp(CurrentWave * 5, 5, 50);
        SpawnManager->CreateZombie(SpawnCount, FVector(523.0f, -293.0f, 101.0f));

        SetTotalZombies(SpawnCount);
    }
}

/** 웨이브 종료 */
void ASevenGameModeBase::EndWave()
{
    if (!bIsNight)
    {
        bIsNight = true;
        if (bUseMiniGame)
        {
            bIsMiniGameActive = true;
        }
        else
        {
            StartNightPhase();
        }
    }
    else
    {
        CurrentWave++;
        if (CurrentWave > WaveLimit)
        {
            return;
        }

        bIsNight = false;
        StartDayPhase();
    }
}

/** 낮 시작 */
void ASevenGameModeBase::StartDayPhase()
{
    bIsNight = false;
    if (DayNightManager) // NULL 체크 추가
    {
        DayNightManager->SetDayNightState(EDayNightState::Day);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[SevenGameModeBase] DayNightManager is NULL!"));
    }
    UpdateDayNightUI();
}

/** 밤 시작 */
void ASevenGameModeBase::StartNightPhase()
{
    bIsNight = true;
    if (DayNightManager) // NULL 체크 추가
    {
        DayNightManager->SetDayNightState(EDayNightState::Night);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[SevenGameModeBase] DayNightManager is NULL!"));
    }
    StartWave();
    UpdateDayNightUI();
}


 

void ASevenGameModeBase::StartMiniGame()
{
    if (!MiniGameClass)
    {
        UE_LOG(LogTemp, Error, TEXT("[SevenGameModeBase] MiniGameClass fail"));
        return;
    }

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC)
    {
        UE_LOG(LogTemp, Error, TEXT("[SevenGameModeBase] player fail"));
        return;
    }

    // 기존 UI 숨기기
    if (ASevenPlayerController* SevenPC = Cast<ASevenPlayerController>(PC))
    {
        if (SevenPC->CurrentWidget)
        {
            SevenPC->CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    // 미니게임 UI 생성 및 추가
    MiniGameInstance = CreateWidget<UMiniGameAvoid>(PC, MiniGameClass);
    if (MiniGameInstance)
    {
        MiniGameInstance->AddToViewport(1); // ZOrder를 1로 설정하여 최상위 UI로 배치

      // // 미니게임 UI에 직접 포커스 설정
      // FSlateApplication::Get().SetUserFocus(0, MiniGameInstance->TakeWidget());

        // 입력 모드를 UI 전용으로 변경
        PC->SetInputMode(FInputModeUIOnly());
        PC->SetShowMouseCursor(true);


        UE_LOG(LogTemp, Warning, TEXT("[SevenGameModeBase] minigame UI done"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[SevenGameModeBase] minigame UI fail!"));
    }
}



 
void ASevenGameModeBase::OnMiniGameCompleted()
{
    UE_LOG(LogTemp, Warning, TEXT("MiniGame Completed - Starting Night Phase"));

    bIsMiniGameActive = false;

   // //  미니게임 UI 제거
   // if (MiniGameInstance)
   // {
   //     MiniGameInstance->RemoveFromParent();
   //     MiniGameInstance = nullptr;
   // }

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        //  기존 UI 복구 (SevenUserWidget을 올바르게 생성)
        if (ASevenPlayerController* SevenPC = Cast<ASevenPlayerController>(PC))
        {
            if (SevenPC->CurrentWidget)
            {
                SevenPC->CurrentWidget->SetVisibility(ESlateVisibility::Visible);
            }
            else
            {
                //  정확한 타입을 지정하여 위젯 생성
                TSubclassOf<USevenUserWidget> SevenHUDClass = LoadClass<USevenUserWidget>(nullptr, TEXT("/Game/UI/BP_SevenUserWidget.BP_SevenUserWidget_C"));
                if (SevenHUDClass)
                {
                    SevenPC->CurrentWidget = CreateWidget<USevenUserWidget>(PC, SevenHUDClass); // USevenUserWidget으로 변경
                    if (SevenPC->CurrentWidget)
                    {
                        SevenPC->CurrentWidget->AddToViewport();
                    }
                }
            }
        }

        //  입력 모드를 게임 모드로 복구
        PC->SetInputMode(FInputModeGameOnly());
        PC->SetShowMouseCursor(false);
    }

    StartNightPhase();
}
 

 void ASevenGameModeBase::EndMiniGame()
 {
     UE_LOG(LogTemp, Warning, TEXT("MiniGame Finished - Starting FPS Game"));
 
     APlayerController* PC = GetWorld()->GetFirstPlayerController();
     if (PC)
     {
         PC->SetInputMode(FInputModeGameOnly());
         PC->SetShowMouseCursor(false);
     }
 
     // 미니게임 UI 제거 (필요하면 추가)
     UUserWidget* MiniGameUI = Cast<UUserWidget>(PC->GetHUD());
     if (MiniGameUI)
     {
         MiniGameUI->RemoveFromParent();
     }
 
     OnMiniGameCompleted();
 
     // 본 게임 시작 로직 (필요하면 추가)
 }
 
 
/** 낮/밤 UI 업데이트 */
void ASevenGameModeBase::UpdateDayNightUI()
{
    if (ASevenPlayerController* PC = Cast<ASevenPlayerController>(GetWorld()->GetFirstPlayerController()))
    {
        if (PC->CurrentWidget)
        {
            PC->CurrentWidget->UpdateDayNightCycle(bIsNight);
        }
    }
}
//----------좀비
void ASevenGameModeBase::SpawnZombies()
{
    // GameState 대신 SevenGameState로 변경
    ASevenGameStateBase* SevenGameState = GetGameState<ASevenGameStateBase>();
    if (!SevenGameState)
    {
        UE_LOG(LogTemp, Error, TEXT("SevenGameState is NULL!"));
        return;
    }

    int32 ZombieCount = FMath::Clamp(CurrentWave * 5, 5, 50);
    SevenGameState->SetTotalZombies(ZombieCount);
    SevenGameState->SetRemainingZombies(ZombieCount);

    for (int32 i = 0; i < ZombieCount; i++)
    {
        FVector SpawnLocation = FVector(FMath::RandRange(-500, 500), FMath::RandRange(-500, 500), 100);
        FRotator SpawnRotation = FRotator::ZeroRotator;
        GetWorld()->SpawnActor<ANBC_Zombie_Base_Character>(ANBC_Zombie_Base_Character::StaticClass(), SpawnLocation, SpawnRotation);
    }

    UE_LOG(LogTemp, Warning, TEXT("[Wave %d] Zombies Spawned: %d"), CurrentWave, ZombieCount);
}

void ASevenGameModeBase::SwitchToDay()
{
    ASevenGameStateBase* SevenGameState = GetGameState<ASevenGameStateBase>();

    if (SevenGameState && SevenGameState->GetRemainingZombies() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("All Zombies Killed! Switching to Day."));

        // 게임 모드에서 낮으로 전환
        TestForceDay();
    }
}

/** 낮으로 강제 변경 */
void ASevenGameModeBase::TestForceDay()
{
    if (DayNightManager)
    {
        DayNightManager->SetDayNightState(EDayNightState::Day);
    }
}

/** 밤으로 강제 변경 */
void ASevenGameModeBase::TestForceNight()
{
    if (DayNightManager)
    {
        DayNightManager->SetDayNightState(EDayNightState::Night);
    }
}


/** 좀비 수 강제 변경 */
void ASevenGameModeBase::TestSetZombieCount(int32 Remaining, int32 Total)
{
    if (ASevenGameStateBase* SevenGS = GetGameState<ASevenGameStateBase>())
    {
        SevenGS->SetTotalZombies(Total);
        SevenGS->SetRemainingZombies(Remaining);
    }
}

/** 전체 좀비 수 설정 */
void ASevenGameModeBase::SetTotalZombies(int32 NewValue)
{
    if (ASevenGameStateBase* SevenGS = GetGameState<ASevenGameStateBase>())
    {
        SevenGS->SetTotalZombies(NewValue);
    }
}