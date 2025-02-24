#include "DayNightManager.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Components/LightComponent.h"
#include "Components/SkyLightComponent.h" 



ADayNightManager::ADayNightManager()
{
    PrimaryActorTick.bCanEverTick = false; // �ʿ信 ���� true��
    CurrentState = EDayNightState::Day; // ���� �� ������ ����

    SkyLightComponent = CreateDefaultSubobject<USkyLightComponent>(TEXT("SkyLightComponent"));
    SkyLightComponent->SetMobility(EComponentMobility::Movable); // Movable ����
    SkyLightComponent->SetIntensity(1.0f); // �⺻�� ����
    RootComponent = SkyLightComponent; // Sky Light�� ��Ʈ�� ����
}

void ADayNightManager::BeginPlay()
{
    Super::BeginPlay();

    // ������ �� �⺻ ���� ���� (��: �� ���·� ����)
    SetDayNightState(CurrentState);
}

void ADayNightManager::SetDayNightState(EDayNightState NewState)
{
    CurrentState = NewState;

    if (NewState == EDayNightState::Day)
    {
        UE_LOG(LogTemp, Log, TEXT("Switching to DAY mode"));

        // ���� �� �¾�� �ϴ� ���� ��� ����
        if (SkyLightComponent)
        {
            SkyLightComponent->SetIntensity(1.0f); // Sky Light �ѱ�
        }

        if (DirectionalLightActor)
        {
            DirectionalLightActor->GetLightComponent()->SetIntensity(3.0f); // �¾� �� �ѱ�
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Switching to NIGHT mode"));

        // ���� �� �¾�� �ϴ� ���� ��� ����
        if (SkyLightComponent)
        {
            SkyLightComponent->SetIntensity(0.0f); // Sky Light ����
        }

        if (DirectionalLightActor)
        {
            DirectionalLightActor->GetLightComponent()->SetIntensity(0.0f); // �¾� �� ����
        }
    }

}




    // (�߰�) Exponential Height Fog, PostProcessVolume �� �ٸ� ���͵� ���۷��� �޾Ƽ� �� ���� ����
    //       (��: �㿡 Fog ���� ���ϰ�, ����(Exposure) ���߱� ��)

