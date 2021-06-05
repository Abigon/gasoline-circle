// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "UI/SGCRestartWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"


void USGCRestartWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (RestartButton)
    {
        RestartButton->OnClicked.AddDynamic(this, &USGCRestartWidget::OnRestart);
    }
}


// ѕолучает название текущего уровн€ и открываем его
void USGCRestartWidget::OnRestart()
{
    const auto CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
    UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}


