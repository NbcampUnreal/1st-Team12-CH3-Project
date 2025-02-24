#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DayNightManager.generated.h"

// 낮/밤 상태를 나누는 Enum (원하는 대로 확장 가능)
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
    // 현재 낮/밤 상태
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DayNight")
    EDayNightState CurrentState;

    // 에디터에서 할당할 Directional Light (주광)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    class ADirectionalLight* DirectionalLightActor;

    // 에디터에서 할당할 Sky Light
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    class ASkyLight* SkyLightActor;

    // 낮/밤 상태 전환 함수
    UFUNCTION(BlueprintCallable, Category = "DayNight")
    void SetDayNightState(EDayNightState NewState);

    // 현재 상태 반환
    UFUNCTION(BlueprintPure, Category = "DayNight")
    EDayNightState GetCurrentState() const { return CurrentState; }
};
