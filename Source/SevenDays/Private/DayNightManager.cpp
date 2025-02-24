#include "DayNightManager.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Components/LightComponent.h"
#include "Components/SkyLightComponent.h" 



ADayNightManager::ADayNightManager()
{
    PrimaryActorTick.bCanEverTick = false; // 필요에 따라 true로
    CurrentState = EDayNightState::Day; // 시작 시 낮으로 설정

    SkyLightComponent = CreateDefaultSubobject<USkyLightComponent>(TEXT("SkyLightComponent"));
    SkyLightComponent->SetMobility(EComponentMobility::Movable); // Movable 설정
    SkyLightComponent->SetIntensity(1.0f); // 기본값 설정
    RootComponent = SkyLightComponent; // Sky Light를 루트로 설정
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

    if (NewState == EDayNightState::Day)
    {
        UE_LOG(LogTemp, Log, TEXT("Switching to DAY mode"));

        // 낮일 때 태양과 하늘 빛을 밝게 설정
        if (SkyLightComponent)
        {
            SkyLightComponent->SetIntensity(1.0f); // Sky Light 켜기
        }

        if (DirectionalLightActor)
        {
            DirectionalLightActor->GetLightComponent()->SetIntensity(3.0f); // 태양 빛 켜기
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Switching to NIGHT mode"));

        // 밤일 때 태양과 하늘 빛을 모두 끄기
        if (SkyLightComponent)
        {
            SkyLightComponent->SetIntensity(0.0f); // Sky Light 끄기
        }

        if (DirectionalLightActor)
        {
            DirectionalLightActor->GetLightComponent()->SetIntensity(0.0f); // 태양 빛 끄기
        }
    }

}




    // (추가) Exponential Height Fog, PostProcessVolume 등 다른 액터도 레퍼런스 받아서 값 조절 가능
    //       (예: 밤에 Fog 색상 진하게, 노출(Exposure) 낮추기 등)

