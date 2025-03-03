#include "SevenGameModeBase.h"
#include "PlayerCharacter.h"
#include "DayNightManager.h"
#include "NBC_SpawnManager.h"
#include "SevenPlayerController.h"
#include "SevenUserWidget.h"
#include "SevenGameStateBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


/** ������: �⺻�� ���� */
ASevenGameModeBase::ASevenGameModeBase()
{
    DefaultPawnClass = APlayerCharacter::StaticClass();

    CurrentWave = 1;
    bIsNight = false;
    bUseMiniGame = false;
    bIsMiniGameActive = false;
}

/** ���� ���� �� ���� (UI ���� ����) */
void ASevenGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    RestartPlayer(GetWorld()->GetFirstPlayerController());  // PlayerStart���� ����

    // ���� �� ���� �� HUD �߰�
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

/** ���� ���� �� ȣ�� (�Ŵ��� �ʱ�ȭ ����) */
void ASevenGameModeBase::StartPlay()
{
    Super::StartPlay();

    // DayNightManager & SpawnManager ã��
    DayNightManager = Cast<ADayNightManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADayNightManager::StaticClass()));
    SpawnManager = Cast<ANBC_SpawnManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ANBC_SpawnManager::StaticClass()));

    // ������ ���� & ù ���̺� ����
    StartDayPhase();
    StartWave();
}



/** ���̺� ���� */
void ASevenGameModeBase::StartWave()
{
    // ���� �ƴϸ� ���� ���� X
    if (!bIsNight)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Wave %d] Skipped - Night Phase Only"), CurrentWave);
        return;
    }

    // ���� ���� ����
    if (SpawnManager)
    {
        int32 SpawnCount = FMath::Clamp(CurrentWave * 5, 5, 50);
        SpawnManager->CreateZombie(SpawnCount, FVector(523.0f, -293.0f, 101.0f));

        SetTotalZombies(SpawnCount);
    }
}

/** ���̺� ���� */
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

    // ������ ����
    ASevenPlayerController* PC = Cast<ASevenPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (PC)
    {
        PC->TestForceDay();
    }

    //���� ���ֱ�


}

/** �� ���� */
void ASevenGameModeBase::StartDayPhase()
{
    bIsNight = false;
    if (DayNightManager)
    {
        DayNightManager->SetDayNightState(EDayNightState::Day);
    }
    UpdateDayNightUI();
}

/** �� ���� */
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

/** �̴ϰ��� �Ϸ� �� �� ���� */
void ASevenGameModeBase::OnMiniGameCompleted()
{
    bIsMiniGameActive = false;
    StartNightPhase();
}

/** ��/�� UI ������Ʈ */
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
//----------����
void ASevenGameModeBase::SpawnZombies()
{
    // GameState ��� SevenGameState�� ����
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

        // ���� ��忡�� ������ ��ȯ
        ASevenPlayerController* PC = Cast<ASevenPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
        if (PC)
        {
            PC->TestForceDay();
        }
    }
}

/** ������ ���� ���� */
void ASevenGameModeBase::TestForceDay()
{
    if (DayNightManager)
    {
        DayNightManager->SetDayNightState(EDayNightState::Day);
    }
}

/** ������ ���� ���� */
void ASevenGameModeBase::TestForceNight()
{
    if (DayNightManager)
    {
        DayNightManager->SetDayNightState(EDayNightState::Night);
    }
}


/** ���� �� ���� ���� */
void ASevenGameModeBase::TestSetZombieCount(int32 Remaining, int32 Total)
{
    if (ASevenGameStateBase* SevenGS = GetGameState<ASevenGameStateBase>())
    {
        SevenGS->SetTotalZombies(Total);
        SevenGS->SetRemainingZombies(Remaining);
    }
}

/** ��ü ���� �� ���� */
void ASevenGameModeBase::SetTotalZombies(int32 NewValue)
{
    if (ASevenGameStateBase* SevenGS = GetGameState<ASevenGameStateBase>())
    {
        SevenGS->SetTotalZombies(NewValue);
    }
}
