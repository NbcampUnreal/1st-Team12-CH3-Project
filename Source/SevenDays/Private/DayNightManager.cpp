#include "DayNightManager.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Components/LightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"
#include "EngineUtils.h"

/** 생성자 */
ADayNightManager::ADayNightManager()
{
    PrimaryActorTick.bCanEverTick = false;
    CurrentState = EDayNightState::Day;
}

/** 게임 시작 시 초기화 */
void ADayNightManager::BeginPlay()
{
    Super::BeginPlay();

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("[DayNightManager] World is NULL! Lighting setup failed."));
        return;
    }

    // DirectionalLight 찾기
    for (TActorIterator<ADirectionalLight> It(World); It; ++It)
    {
        DirectionalLightActor = *It;
        UE_LOG(LogTemp, Warning, TEXT("[DayNightManager] DirectionalLight found: %s"), *DirectionalLightActor->GetName());
        break;
    }

    // SkyLight 찾기
    for (TActorIterator<ASkyLight> It(World); It; ++It)
    {
        SkyLightActor = *It;
        UE_LOG(LogTemp, Warning, TEXT("[DayNightManager] SkyLight found: %s"), *SkyLightActor->GetName());
        break;
    }

    // BP_Sky_Sphere 찾기 (태그 기반)
    for (TActorIterator<AActor> It(World, AActor::StaticClass()); It; ++It)
    {
        if (It->ActorHasTag(FName("BP_Sky_Sphere"))) // BP_Sky_Sphere에 "BP_Sky_Sphere" 태그 추가 필요
        {
            BP_Sky_Sphere = *It;
            UE_LOG(LogTemp, Warning, TEXT("[DayNightManager] BP_Sky_Sphere found: %s"), *BP_Sky_Sphere->GetName());
            break;
        }
    }

    // BP_Sky_Sphere가 NULL이면 강제로 찾기
    if (!BP_Sky_Sphere)
    {
        BP_Sky_Sphere = UGameplayStatics::GetActorOfClass(GetWorld(), AActor::StaticClass());
        if (BP_Sky_Sphere)
        {
            UE_LOG(LogTemp, Warning, TEXT("[DayNightManager] Found BP_Sky_Sphere using GetActorOfClass."));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[DayNightManager] BP_Sky_Sphere NOT found! Check if it's placed in the level."));
        }
    }
}

/** 낮/밤 상태 변경 */
void ADayNightManager::SetDayNightState(EDayNightState NewState)
{
    CurrentState = NewState;
    UpdateLighting();   // 조명 업데이트
    UpdateSkySphere();  // BP_Sky_Sphere 업데이트

    UE_LOG(LogTemp, Warning, TEXT("[DayNightManager] DayNight state changed: %s"),
        (NewState == EDayNightState::Day) ? TEXT("Day") : TEXT("Night"));
}

/** 조명 업데이트 */
void ADayNightManager::UpdateLighting()
{
    if (DirectionalLightActor && DirectionalLightActor->GetLightComponent())
    {
        if (CurrentState == EDayNightState::Day)
        {
            DirectionalLightActor->SetActorRotation(FRotator(-45.f, 0.f, 0.f));
            DirectionalLightActor->GetLightComponent()->SetIntensity(3.0f); // 낮에는 강한 빛
        }
        else
        {
            DirectionalLightActor->SetActorRotation(FRotator(180.f, 0.f, 0.f));
            DirectionalLightActor->GetLightComponent()->SetIntensity(0.1f); // 밤에는 매우 약한 빛
        }
    }

    if (SkyLightActor && SkyLightActor->GetLightComponent())
    {
        USkyLightComponent* SkyLightComp = SkyLightActor->GetLightComponent();

        if (CurrentState == EDayNightState::Day)
        {
            SkyLightComp->SetIntensity(1.0f); // 낮에는 기본 밝기 유지
        }
        else
        {
            SkyLightComp->SetIntensity(0.05f); // 밤에는 전체적으로 어둡게
        }

        SkyLightComp->MarkRenderStateDirty();
    }
}

/** 하늘 구체(BP_Sky_Sphere) 업데이트 */

void ADayNightManager::UpdateSkySphere()
{
    if (!BP_Sky_Sphere)
    {
        UE_LOG(LogTemp, Error, TEXT("[DayNightManager] BP_Sky_Sphere is NULL. SkySphere update failed."));
        return;
    }

    float NewSunHeight = (CurrentState == EDayNightState::Day) ? 0.6f : -1.0f;
    UE_LOG(LogTemp, Warning, TEXT("[DayNightManager] Setting SunHeight to: %f"), NewSunHeight);

    // 블루프린트의 UpdateSunHeight() 함수 호출
    FOutputDeviceNull Ar;
    FString Command = FString::Printf(TEXT("UpdateSunHeight %f"), NewSunHeight);
    BP_Sky_Sphere->CallFunctionByNameWithArguments(*Command, Ar, nullptr, true);

    UE_LOG(LogTemp, Warning, TEXT("[DayNightManager] Called UpdateSunHeight in BP_Sky_Sphere"));

    // SkySphere의 Sun 방향 업데이트 (기존 블루프린트 함수 호출)
    BP_Sky_Sphere->CallFunctionByNameWithArguments(TEXT("UpdateSunDirection"), Ar, nullptr, true);

    UE_LOG(LogTemp, Warning, TEXT("[DayNightManager] Called UpdateSunDirection on BP_Sky_Sphere"));
}
