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
    // 로딩 화면 블루프린트 로드
    static ConstructorHelpers::FClassFinder<UUserWidget> LoadingScreenBP(TEXT("/Game/UI/WB_LoadingScreen.WB_LoadingScreen_C"));
    if (LoadingScreenBP.Succeeded())
    {
        LoadingScreenClass = LoadingScreenBP.Class;
    }
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

     // "PlayerSpawnPoint"라는 이름을 가진 액터들을 모두 찾기
     UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PlayerSpawnPoint"), PlayerSpawnPoints);

     if (PlayerSpawnPoints.Num() == 0)
     {
         UE_LOG(LogTemp, Error, TEXT("No PlayerSpawnPoints found in the level!"));
     }
     else
     {
         UE_LOG(LogTemp, Warning, TEXT("Found %d PlayerSpawnPoints"), PlayerSpawnPoints.Num());
     }

     SpawnManager = Cast<ANBC_SpawnManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ANBC_SpawnManager::StaticClass()));

     // 첫 웨이브 스폰 위치 적용
     SetPlayerSpawnLocation();


}

FVector ASevenGameModeBase::GetSpawnLocationForWave(int32 Wave)
{
    if (SpawnManager->PlayerSpawnPointArray.Num() < 3)
    {
        UE_LOG(LogTemp, Error, TEXT("Not enough PlayerSpawnPoints found!"));
        return FVector(0.0f, 0.0f, 200.0f);
    }

    // 1~6 웨이브: 현재 웨이브 수를 기준으로 번갈아 가면서 선택
    if (Wave >= 1 && Wave <= 2)
    {
        int32 SpawnIndex = (Wave % 2 == 0) ? 0 : 1; // 짝수 웨이브: 1번 스폰 지역, 홀수 웨이브: 2번 스폰 지역
        SpawnManager->SetSpawnPoint(SpawnIndex);
        return SpawnManager->PlayerSpawnPointArray[SpawnIndex]->GetActorLocation();
        //PlayerSpawnPoints[SpawnIndex]->GetActorLocation();

    }
    // 7 웨이브: 3번 스폰 지역 고정
    else if (Wave == 3)
    {
        SpawnManager->SetSpawnPoint(2);
        //SpawnManager->CreateBoss(SpawnManager->PlayerSpawnPointArray[2]->GetActorLocation());
        return SpawnManager->PlayerSpawnPointArray[2]->GetActorLocation();
    }

    return FVector(0.0f, 0.0f, 200.0f); // 기본값
}


void ASevenGameModeBase::SetPlayerSpawnLocation()
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC || !PC->GetPawn()) return;

    // 현재 웨이브에 맞는 스폰 위치 가져오기
    FVector SpawnLocation = GetSpawnLocationForWave(CurrentWave);

    // 플레이어 이동
    PC->GetPawn()->SetActorLocation(SpawnLocation);
    UE_LOG(LogTemp, Warning, TEXT("Player moved to Wave %d location: %s"), CurrentWave, *SpawnLocation.ToString());
}



void ASevenGameModeBase::OnEnemyKilled()
{
    if (!KillConfirmUI) return;

    KillConfirmUI->SetVisibility(ESlateVisibility::Visible);

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

    // 현재 남은 좀비 수 확인
    ASevenGameStateBase* SevenGameState = GetGameState<ASevenGameStateBase>();
    if (SevenGameState && SevenGameState->GetRemainingZombies() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("All Zombies Killed! Showing Loading Screen..."));

        // 1. 로딩 화면 표시
        ShowLoadingScreen();

        // 2. 2초 후 미니게임 시작
        FTimerHandle MiniGameTimer;
        GetWorldTimerManager().SetTimer(MiniGameTimer, [this]() 
            {
                HideLoadingScreen(); // 로딩 화면 숨기기
                StartMiniGame(); // 미니게임 시작
            }, 2.0f, false); // 2초 대기

    }
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
        if (CurrentWave == 3) {
            SpawnManager->CreateBoss(FVector(523.0f, -293.0f, 101.0f));
            return;
        }
        
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

        
      //  bIsNight = false;

        OnEnemyKilled();
        StartDayPhase();
        SetPlayerSpawnLocation();  //  웨이브 이동 시 플레이어 위치 변경

        SpawnManager->ClearZombie(); // 좀비 청소


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
                UE_LOG(LogTemp, Warning, TEXT("!!!!!!!!!!!!!NULL!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
                SevenPC->CurrentWidget->SetVisibility(ESlateVisibility::Visible);

            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("!!!!!!!!!!!!!NULL2222222222222!!!!!!!!!!!!!!!!!!!!!"));
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
    ASevenGameStateBase* SevenGameState = GetGameState<ASevenGameStateBase>();
    if (!SevenGameState)
    {
        UE_LOG(LogTemp, Error, TEXT("SevenGameState is NULL!"));
        return;
    }

    // 7 웨이브: 보스 좀비만 3번 스폰 지역에서 생성
    if (CurrentWave == 7)
    {
        SpawnManager->CreateBoss(FVector::ZeroVector);
        return;
    }

    // 1~6 웨이브: 일반 좀비 스폰
    int32 ZombieCount = FMath::Clamp(CurrentWave * 5, 5, 50);
    SevenGameState->SetTotalZombies(ZombieCount);
    SevenGameState->SetRemainingZombies(ZombieCount);

    for (int32 i = 0; i < ZombieCount; i++)
    {
        FVector SpawnLocation = GetSpawnLocationForWave(CurrentWave);
        FRotator SpawnRotation = FRotator::ZeroRotator;
        GetWorld()->SpawnActor<ANBC_Zombie_Base_Character>(ANBC_Zombie_Base_Character::StaticClass(), SpawnLocation, SpawnRotation);
    }

    UE_LOG(LogTemp, Warning, TEXT("[Wave %d] Zombies Spawned: %d"), CurrentWave, ZombieCount);
}


void ASevenGameModeBase::SpawnBossZombie()
{
    if (!BossZombieClass) return;

    FVector SpawnLocation = GetSpawnLocationForWave(7); // 7 웨이브 스폰 위치 사용
    FRotator SpawnRotation = FRotator::ZeroRotator;

    ANBC_Zombie_Boss_Character* BossZombie = GetWorld()->SpawnActor<ANBC_Zombie_Boss_Character>(BossZombieClass, SpawnLocation, SpawnRotation);

    if (BossZombie)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Wave 7] Boss Zombie Spawned at %s"), *SpawnLocation.ToString());
    }
}

void ASevenGameModeBase::EndCredit()
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        EndCreditins = CreateWidget<UUserWidget>(PC, EndCreditref);
        if (EndCreditins)
        {
            EndCreditins->AddToViewport(10); // UI 최상단에 추가

            // 입력을 UI 모드로 변경 (플레이어 입력 차단)
            PC->SetInputMode(FInputModeUIOnly());
            PC->SetShowMouseCursor(true);
        }
    }
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

//로딩화면
void ASevenGameModeBase::ShowLoadingScreen()
{
    if (!LoadingScreenClass) return;

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;

    // 기존 로딩 화면이 있으면 제거
    if (LoadingScreenInstance)
    {
        LoadingScreenInstance->RemoveFromParent();
        LoadingScreenInstance = nullptr;
    }

    // 새로운 로딩 화면 생성
    LoadingScreenInstance = CreateWidget<UUserWidget>(PC, LoadingScreenClass);
    if (LoadingScreenInstance)
    {
        LoadingScreenInstance->AddToViewport(10); // 가장 위에 표시
    }
}

void ASevenGameModeBase::HideLoadingScreen()
{
    if (LoadingScreenInstance)
    {
        LoadingScreenInstance->RemoveFromParent();
        LoadingScreenInstance = nullptr;
    }
}

