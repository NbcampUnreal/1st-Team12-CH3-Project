#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DayNightManager.generated.h"

/**
 * 낮과 밤 상태를 정의하는 열거형 (Day: 낮, Night: 밤)
 */
UENUM(BlueprintType)
enum class EDayNightState : uint8
{
    Day UMETA(DisplayName = "Day"),
    Night UMETA(DisplayName = "Night")
};

/**
 * 낮과 밤을 관리하는 매니저 클래스
 */
UCLASS()
class SEVENDAYS_API ADayNightManager : public AActor
{
    GENERATED_BODY()

public:
    /** 생성자 */
    ADayNightManager();

protected:
    /** 게임 시작 시 초기화 */
    virtual void BeginPlay() override;

public:
    /** 현재 낮/밤 상태 (낮: Day, 밤: Night) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DayNight")
    EDayNightState CurrentState;

    /** DirectionalLight (태양 조명) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    class ADirectionalLight* DirectionalLightActor;

    /** SkyLight (환경광 조명) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    class ASkyLight* SkyLightActor;

    /** 하늘 구체 (BP_Sky_Sphere) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky")
    AActor* BP_Sky_Sphere;

    /**
     * 낮/밤 상태 변경 (즉시 변경)
     * @param NewState 변경할 상태 (Day/Night)
     */
    UFUNCTION(BlueprintCallable, Category = "DayNight")
    void SetDayNightState(EDayNightState NewState);

    /** 조명을 업데이트하는 함수 */
    void UpdateLighting();

    /** 하늘 구체(BP_Sky_Sphere)를 업데이트하는 함수 */
    void UpdateSkySphere();
};
