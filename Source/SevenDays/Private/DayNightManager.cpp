#include "DayNightManager.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Components/LightComponent.h"
#include "Components/SkyLightComponent.h" // ★ 이 부분 추가


ADayNightManager::ADayNightManager()
{
    PrimaryActorTick.bCanEverTick = false; // 필요에 따라 true로
    CurrentState = EDayNightState::Day; // 시작 시 낮으로 설정
}

void ADayNightManager::BeginPlay()
{
    Super::BeginPlay();

    // 시작할 때 기본 상태 지정 (예: 낮 상태로 시작)
    SetDayNightState(CurrentState);
}

void ADayNightManager::SetDayNightState(EDayNightState NewState)
{
    CurrentState = NewState;

    // 1) 만약 Day(낮) 상태
    if (NewState == EDayNightState::Day)
    {
        UE_LOG(LogTemp, Log, TEXT("Switching to DAY mode"));

        // Directional Light(태양광) 설정
        if (DirectionalLightActor)
        {
            // 예: 태양이 위쪽에 있는 각도
            DirectionalLightActor->SetActorRotation(FRotator(-45.f, 0.f, 0.f));

            // 빛 세기 (낮: 좀 세게)
            DirectionalLightActor->GetLightComponent()->SetIntensity(10000.f);

            // 빛 색상 (따뜻한 빛)
            DirectionalLightActor->GetLightComponent()->SetLightColor(FLinearColor(1.f, 0.95f, 0.9f));
        }

        // Sky Light 세기 (낮에는 1.0 정도)
        if (SkyLightActor)
        {
            SkyLightActor->GetLightComponent()->SetIntensity(1.0f);
        }
    }
    // 2) Night(밤) 상태
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Switching to NIGHT mode"));

        if (DirectionalLightActor)
        {
            // 예: 태양을 지평선 아래로
            DirectionalLightActor->SetActorRotation(FRotator(180.f, 0.f, 0.f));

            // 밤에는 빛 세기를 약하게 (혹은 0으로 끄기)
            DirectionalLightActor->GetLightComponent()->SetIntensity(0.5f);

            // 달빛스럽게 약간 푸른 톤
            DirectionalLightActor->GetLightComponent()->SetLightColor(FLinearColor(0.7f, 0.7f, 1.f));
        }

        // 밤에는 Sky Light도 약하게
        if (SkyLightActor)
        {
            SkyLightActor->GetLightComponent()->SetIntensity(0.2f);
        }
    }

    // (추가) Exponential Height Fog, PostProcessVolume 등 다른 액터도 레퍼런스 받아서 값 조절 가능
    //       (예: 밤에 Fog 색상 진하게, 노출(Exposure) 낮추기 등)
}
