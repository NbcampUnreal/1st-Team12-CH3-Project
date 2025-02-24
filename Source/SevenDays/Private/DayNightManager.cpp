#include "DayNightManager.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Components/LightComponent.h"
#include "Components/SkyLightComponent.h" // �� �� �κ� �߰�


ADayNightManager::ADayNightManager()
{
    PrimaryActorTick.bCanEverTick = false; // �ʿ信 ���� true��
    CurrentState = EDayNightState::Day; // ���� �� ������ ����
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

    // 1) ���� Day(��) ����
    if (NewState == EDayNightState::Day)
    {
        UE_LOG(LogTemp, Log, TEXT("Switching to DAY mode"));

        // Directional Light(�¾籤) ����
        if (DirectionalLightActor)
        {
            // ��: �¾��� ���ʿ� �ִ� ����
            DirectionalLightActor->SetActorRotation(FRotator(-45.f, 0.f, 0.f));

            // �� ���� (��: �� ����)
            DirectionalLightActor->GetLightComponent()->SetIntensity(10000.f);

            // �� ���� (������ ��)
            DirectionalLightActor->GetLightComponent()->SetLightColor(FLinearColor(1.f, 0.95f, 0.9f));
        }

        // Sky Light ���� (������ 1.0 ����)
        if (SkyLightActor)
        {
            SkyLightActor->GetLightComponent()->SetIntensity(1.0f);
        }
    }
    // 2) Night(��) ����
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Switching to NIGHT mode"));

        if (DirectionalLightActor)
        {
            // ��: �¾��� ���� �Ʒ���
            DirectionalLightActor->SetActorRotation(FRotator(180.f, 0.f, 0.f));

            // �㿡�� �� ���⸦ ���ϰ� (Ȥ�� 0���� ����)
            DirectionalLightActor->GetLightComponent()->SetIntensity(0.5f);

            // �޺������� �ణ Ǫ�� ��
            DirectionalLightActor->GetLightComponent()->SetLightColor(FLinearColor(0.7f, 0.7f, 1.f));
        }

        // �㿡�� Sky Light�� ���ϰ�
        if (SkyLightActor)
        {
            SkyLightActor->GetLightComponent()->SetIntensity(0.2f);
        }
    }

    // (�߰�) Exponential Height Fog, PostProcessVolume �� �ٸ� ���͵� ���۷��� �޾Ƽ� �� ���� ����
    //       (��: �㿡 Fog ���� ���ϰ�, ����(Exposure) ���߱� ��)
}
