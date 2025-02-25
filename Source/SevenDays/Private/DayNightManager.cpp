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
    // 낮 상태의 SunHeight는 BP_Sky_Sphere 기준 0.6, 밤은 -1
    CurrentSunHeight = 0.6f;
    TargetSunHeight = -1.f;
    SunHeightInterpSpeed = 0.01f;  // 기존 변수, 사용 안할 수도 있음
    bIsDaytime = true; // 기본은 낮

    TransitionElapsedTime = 0.f;
    TransitionTotalTime = 1.f; // 초기값, 실제 전환 시 DayDuration으로 설정
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

    // DirectionalLight 찾기
    for (TActorIterator<ADirectionalLight> It(World); It; ++It)
    {
        DirectionalLightActor = *It;
        break;
    }

    // SkyLight 찾기
    for (TActorIterator<ASkyLight> It(World); It; ++It)
    {
        SkyLightActor = *It;
        break;
    }



    // "BP_Sky_Sphere" 태그가 붙은 액터 찾기
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

    // SkySphere 머티리얼 업데이트
    SkySphere->CallFunctionByNameWithArguments(TEXT("UpdateMaterial"), OutputDevice, nullptr, true);

    // SkyLight 업데이트
    if (SkyLightActor && SkyLightActor->GetLightComponent())
    {
        SkyLightActor->GetLightComponent()->MarkRenderStateDirty();
    }

    UE_LOG(LogTemp, Warning, TEXT("SkySphere SunHeight updated to: %f"), NewSunHeight);
}


void ADayNightManager::SetDayNightState(EDayNightState NewState, float DayDuration)
{
    CurrentState = NewState;
    UpdateLighting(); // 방향성 조명 업데이트

    if (NewState == EDayNightState::Day)
    {
        // 낮일 때: 태양 높이를 0.6에서 -1.0로 서서히 변화
        CurrentSunHeight = 0.6f;
        TargetSunHeight = -1.0f;
        SunHeightInterpSpeed = 0.8f / DayDuration;

        GetWorldTimerManager().SetTimer(SmoothTransitionHandle, this, &ADayNightManager::SmoothTransitionSunHeight, 0.1f, true);
    }
    else
    {
        // 타이머 중지 후 즉시 밤 상태 적용
        GetWorldTimerManager().ClearTimer(SmoothTransitionHandle);
        SetSunHeight(-1.0f);
    }

    //  SkySphere 머티리얼 강제 업데이트
    if (SkySphere)
    {
        FOutputDeviceNull OutputDevice;
        SkySphere->CallFunctionByNameWithArguments(TEXT("UpdateMaterial"), OutputDevice, nullptr, true);
    }

    //  SkyLight 업데이트 (조명 반영)
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
    // 필요 시 SkySphere 업데이트 로직 추가
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
            Params.SunHeight = bIsDaytime ? 0.6f : -1.0f; // 낮이면 0.6, 밤이면 -1.0

            SkySphere->ProcessEvent(SetSunHeightFunc, &Params);
        }
    }
}
