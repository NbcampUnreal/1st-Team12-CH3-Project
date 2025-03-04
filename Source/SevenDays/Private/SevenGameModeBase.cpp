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
    UE_LOG(LogTemp, Warning, TEXT("[Wave %d] END - %s"), CurrentWave, bIsNight ? TEXT("Night") : TEXT("Day"));

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
            UE_LOG(LogTemp, Warning, TEXT("[SevenGameModeBase] All waves finished!"));
            return;
        }

        bIsNight = false;
        StartDayPhase();


    }

    // 낮으로 변경
    ASevenPlayerController* PC = Cast<ASevenPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (PC)
    {
        PC->TestForceDay();
    }

    //좀비 없애기


}

/** 낮 시작 */
void ASevenGameModeBase::StartDayPhase()
{
    bIsNight = false;
    if (DayNightManager)
    {
        DayNightManager->SetDayNightState(EDayNightState::Day);
    }
    UpdateDayNightUI();
}

/** 밤 시작 */
void ASevenGameModeBase::StartNightPhase()
{
    bIsNight = true;
    if (DayNightManager)
    {
        DayNightManager->SetDayNightState(EDayNightState::Night);
    }
    StartWave();
    UpdateDayNightUI();
}

/** 미니게임 완료 후 밤 시작 */
void ASevenGameModeBase::OnMiniGameCompleted()
{
    bIsMiniGameActive = false;
    StartNightPhase();
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
        ASevenPlayerController* PC = Cast<ASevenPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
        if (PC)
        {
            PC->TestForceDay();
        }
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
