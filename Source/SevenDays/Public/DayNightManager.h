#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DayNightManager.generated.h"

/**
 * ���� �� ���¸� �����ϴ� ������ (Day: ��, Night: ��)
 */
UENUM(BlueprintType)
enum class EDayNightState : uint8
{
    Day UMETA(DisplayName = "Day"),
    Night UMETA(DisplayName = "Night")
};

/**
 * ���� ���� �����ϴ� �Ŵ��� Ŭ����
 */
UCLASS()
class SEVENDAYS_API ADayNightManager : public AActor
{
    GENERATED_BODY()

public:
    /** ������ */
    ADayNightManager();

protected:
    /** ���� ���� �� �ʱ�ȭ */
    virtual void BeginPlay() override;

public:
    /** ���� ��/�� ���� (��: Day, ��: Night) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DayNight")
    EDayNightState CurrentState;

    /** DirectionalLight (�¾� ����) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    class ADirectionalLight* DirectionalLightActor;

    /** SkyLight (ȯ�汤 ����) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    class ASkyLight* SkyLightActor;

    /** �ϴ� ��ü (BP_Sky_Sphere) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky")
    AActor* BP_Sky_Sphere;

    /**
     * ��/�� ���� ���� (��� ����)
     * @param NewState ������ ���� (Day/Night)
     */
    UFUNCTION(BlueprintCallable, Category = "DayNight")
    void SetDayNightState(EDayNightState NewState);

    /** ������ ������Ʈ�ϴ� �Լ� */
    void UpdateLighting();

    /** �ϴ� ��ü(BP_Sky_Sphere)�� ������Ʈ�ϴ� �Լ� */
    void UpdateSkySphere();
};
