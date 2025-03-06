#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "GameOverWidget.generated.h"

UCLASS()
class SEVENDAYS_API UGameOverWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // 블루프린트에서 연결할 버튼들
    UPROPERTY(meta = (BindWidget))
    UButton* GameStartButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitButton;
};
