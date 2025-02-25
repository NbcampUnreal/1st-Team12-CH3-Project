#include "DayNightManager.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Components/LightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"
#include "EngineUtils.h"

ADayNightManager::ADayNightManager()
{
    PrimaryActorTick.bCanEverTick = false;
    CurrentState = EDayNightState::Day;
    // �� ������ SunHeight�� BP_Sky_Sphere ���� 0.6, ���� -1
    CurrentSunHeight = 0.6f;
    TargetSunHeight = -1.f;
    SunHeightInterpSpeed = 0.01f;  // ���� ����, ��� ���� ���� ����
    bIsDaytime = true; // �⺻�� ��

    TransitionElapsedTime = 0.f;
    TransitionTotalTime = 1.f; // �ʱⰪ, ���� ��ȯ �� DayDuration���� ����
}

void ADayNightManager::BeginPlay()
{
    Super::BeginPlay();

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("World is NULL! Lighting setup failed."));
        return;
    }

    // DirectionalLight ã��
    for (TActorIterator<ADirectionalLight> It(World); It; ++It)
    {
        DirectionalLightActor = *It;
        break;
    }

    // SkyLight ã��
    for (TActorIterator<ASkyLight> It(World); It; ++It)
    {
        SkyLightActor = *It;
        break;
    }



    // "BP_Sky_Sphere" �±װ� ���� ���� ã��
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsWithTag(World, FName("BP_Sky_Sphere"), FoundActors);

    if (FoundActors.Num() > 0)
    {
        SkySphere = FoundActors[0];
        UE_LOG(LogTemp, Warning, TEXT("BP_Sky_Sphere found: %s"), *SkySphere->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No BP_Sky_Sphere found in the scene!"));
    }
}


void ADayNightManager::SetSunHeight(float NewSunHeight)
{
    CurrentSunHeight = NewSunHeight;
    if (!SkySphere)
    {
        UE_LOG(LogTemp, Error, TEXT("SkySphere is NULL! Cannot update SunHeight."));
        return;
    }

    FOutputDeviceNull OutputDevice;
    FString Command = FString::Printf(TEXT("SetSunHeight %f"), NewSunHeight);
    SkySphere->CallFunctionByNameWithArguments(*Command, OutputDevice, nullptr, true);

    // SkySphere ��Ƽ���� ������Ʈ
    SkySphere->CallFunctionByNameWithArguments(TEXT("UpdateMaterial"), OutputDevice, nullptr, true);

    // SkyLight ������Ʈ
    if (SkyLightActor && SkyLightActor->GetLightComponent())
    {
        SkyLightActor->GetLightComponent()->MarkRenderStateDirty();
    }

    UE_LOG(LogTemp, Warning, TEXT("SkySphere SunHeight updated to: %f"), NewSunHeight);
}


void ADayNightManager::SetDayNightState(EDayNightState NewState, float DayDuration)
{
    CurrentState = NewState;
    UpdateLighting(); // ���⼺ ���� ������Ʈ

    if (NewState == EDayNightState::Day)
    {
        // ���� ��: �¾� ���̸� 0.6���� -1.0�� ������ ��ȭ
        CurrentSunHeight = 0.6f;
        TargetSunHeight = -1.0f;
        SunHeightInterpSpeed = 0.8f / DayDuration;

        GetWorldTimerManager().SetTimer(SmoothTransitionHandle, this, &ADayNightManager::SmoothTransitionSunHeight, 0.1f, true);
    }
    else
    {
        // Ÿ�̸� ���� �� ��� �� ���� ����
        GetWorldTimerManager().ClearTimer(SmoothTransitionHandle);
        SetSunHeight(-1.0f);
    }

    //  SkySphere ��Ƽ���� ���� ������Ʈ
    if (SkySphere)
    {
        FOutputDeviceNull OutputDevice;
        SkySphere->CallFunctionByNameWithArguments(TEXT("UpdateMaterial"), OutputDevice, nullptr, true);
    }

    //  SkyLight ������Ʈ (���� �ݿ�)
    if (SkyLightActor && SkyLightActor->GetLightComponent())
    {
        SkyLightActor->GetLightComponent()->MarkRenderStateDirty();
    }
}


void ADayNightManager::UpdateLighting()
{
    if (CurrentState == EDayNightState::Day)
    {
        if (DirectionalLightActor)
        {
            DirectionalLightActor->SetActorRotation(FRotator(-45.f, 0.f, 0.f));
        }
    }
    else
    {
        if (DirectionalLightActor)
        {
            DirectionalLightActor->SetActorRotation(FRotator(180.f, 0.f, 0.f));
        }
    }
}

void ADayNightManager::UpdateSkySphere()
{
    // �ʿ� �� SkySphere ������Ʈ ���� �߰�
}

void ADayNightManager::SmoothTransitionSunHeight()
{
    if (!SkySphere)
    {
        GetWorldTimerManager().ClearTimer(SmoothTransitionHandle);
        return;
    }

    CurrentSunHeight = FMath::Lerp(CurrentSunHeight, TargetSunHeight, SunHeightInterpSpeed);
    SetSunHeight(CurrentSunHeight);

    if (FMath::IsNearlyEqual(CurrentSunHeight, TargetSunHeight, 0.05f))
    {
        GetWorldTimerManager().ClearTimer(SmoothTransitionHandle);
    }
}

void ADayNightManager::UpdateSunHeight()
{
    if (SkySphere)
    {
        UFunction* SetSunHeightFunc = SkySphere->FindFunction(FName("SetSunHeight"));
        if (SetSunHeightFunc)
        {
            struct FSunHeightParams
            {
                float SunHeight;
            };

            FSunHeightParams Params;
            Params.SunHeight = bIsDaytime ? 0.6f : -1.0f; // ���̸� 0.6, ���̸� -1.0

            SkySphere->ProcessEvent(SetSunHeightFunc, &Params);
        }
    }
}
