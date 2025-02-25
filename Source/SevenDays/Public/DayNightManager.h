#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DayNightManager.generated.h"

// ��/�� ���¸� ������ Enum (���ϴ� ��� Ȯ�� ����)
UENUM(BlueprintType)
enum class EDayNightState : uint8
{
    Day UMETA(DisplayName = "Day"),
    Night UMETA(DisplayName = "Night")
};

UCLASS()
class ADayNightManager : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ADayNightManager();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // ���� ��/�� ����
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DayNight")
    EDayNightState CurrentState;

    // �����Ϳ��� �Ҵ��� Directional Light (�ֱ�)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    class ADirectionalLight* DirectionalLightActor;

    // �����Ϳ��� �Ҵ��� Sky Light
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    class ASkyLight* SkyLightActor;

    // ��/�� ���� ��ȯ �Լ�
    UFUNCTION(BlueprintCallable, Category = "DayNight")
    void SetDayNightState(EDayNightState NewState);

    // ���� ���� ��ȯ
    UFUNCTION(BlueprintPure, Category = "DayNight")
    EDayNightState GetCurrentState() const { return CurrentState; }
};
