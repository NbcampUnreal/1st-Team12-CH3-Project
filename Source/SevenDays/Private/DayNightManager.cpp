#include "DayNightManager.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Components/LightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"
#include "EngineUtils.h"

/** ������ */
ADayNightManager::ADayNightManager()
{
    PrimaryActorTick.bCanEverTick = false;
    CurrentState = EDayNightState::Day;
}

/** ���� ���� �� �ʱ�ȭ */
void ADayNightManager::BeginPlay()
{
    Super::BeginPlay();

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("[DayNightManager] World is NULL! Lighting setup failed."));
        return;
    }

    // DirectionalLight ã��
    for (TActorIterator<ADirectionalLight> It(World); It; ++It)
    {
        DirectionalLightActor = *It;
        UE_LOG(LogTemp, Warning, TEXT("[DayNightManager] DirectionalLight found: %s"), *DirectionalLightActor->GetName());
        break;
    }

    // SkyLight ã��
    for (TActorIterator<ASkyLight> It(World); It; ++It)
    {
        SkyLightActor = *It;
        UE_LOG(LogTemp, Warning, TEXT("[DayNightManager] SkyLight found: %s"), *SkyLightActor->GetName());
        break;
    }

    // BP_Sky_Sphere ã�� (�±� ���)
    for (TActorIterator<AActor> It(World, AActor::StaticClass()); It; ++It)
    {
        if (It->ActorHasTag(FName("BP_Sky_Sphere"))) // BP_Sky_Sphere�� "BP_Sky_Sphere" �±� �߰� �ʿ�
        {
            BP_Sky_Sphere = *It;
            UE_LOG(LogTemp, Warning, TEXT("[DayNightManager] BP_Sky_Sphere found: %s"), *BP_Sky_Sphere->GetName());
            break;
        }
    }

    // BP_Sky_Sphere�� NULL�̸� ������ ã��
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

/** ��/�� ���� ���� */
void ADayNightManager::SetDayNightState(EDayNightState NewState)
{
    CurrentState = NewState;
    UpdateLighting();   // ���� ������Ʈ
    UpdateSkySphere();  // BP_Sky_Sphere ������Ʈ

    UE_LOG(LogTemp, Warning, TEXT("[DayNightManager] DayNight state changed: %s"),
        (NewState == EDayNightState::Day) ? TEXT("Day") : TEXT("Night"));
}

/** ���� ������Ʈ */
void ADayNightManager::UpdateLighting()
{
    if (DirectionalLightActor && DirectionalLightActor->GetLightComponent())
    {
        if (CurrentState == EDayNightState::Day)
        {
            DirectionalLightActor->SetActorRotation(FRotator(-45.f, 0.f, 0.f));
            DirectionalLightActor->GetLightComponent()->SetIntensity(3.0f); // ������ ���� ��
        }
        else
        {
            DirectionalLightActor->SetActorRotation(FRotator(180.f, 0.f, 0.f));
            DirectionalLightActor->GetLightComponent()->SetIntensity(0.1f); // �㿡�� �ſ� ���� ��
        }
    }

    if (SkyLightActor && SkyLightActor->GetLightComponent())
    {
        USkyLightComponent* SkyLightComp = SkyLightActor->GetLightComponent();

        if (CurrentState == EDayNightState::Day)
        {
            SkyLightComp->SetIntensity(1.0f); // ������ �⺻ ��� ����
        }
        else
        {
            SkyLightComp->SetIntensity(0.05f); // �㿡�� ��ü������ ��Ӱ�
        }

        SkyLightComp->MarkRenderStateDirty();
    }
}

/** �ϴ� ��ü(BP_Sky_Sphere) ������Ʈ */

void ADayNightManager::UpdateSkySphere()
{
    if (!BP_Sky_Sphere)
    {
        UE_LOG(LogTemp, Error, TEXT("[DayNightManager] BP_Sky_Sphere is NULL. SkySphere update failed."));
        return;
    }

    float NewSunHeight = (CurrentState == EDayNightState::Day) ? 0.6f : -1.0f;
    UE_LOG(LogTemp, Warning, TEXT("[DayNightManager] Setting SunHeight to: %f"), NewSunHeight);

    // �������Ʈ�� UpdateSunHeight() �Լ� ȣ��
    FOutputDeviceNull Ar;
    FString Command = FString::Printf(TEXT("UpdateSunHeight %f"), NewSunHeight);
    BP_Sky_Sphere->CallFunctionByNameWithArguments(*Command, Ar, nullptr, true);

    UE_LOG(LogTemp, Warning, TEXT("[DayNightManager] Called UpdateSunHeight in BP_Sky_Sphere"));

    // SkySphere�� Sun ���� ������Ʈ (���� �������Ʈ �Լ� ȣ��)
    BP_Sky_Sphere->CallFunctionByNameWithArguments(TEXT("UpdateSunDirection"), Ar, nullptr, true);

    UE_LOG(LogTemp, Warning, TEXT("[DayNightManager] Called UpdateSunDirection on BP_Sky_Sphere"));
}
