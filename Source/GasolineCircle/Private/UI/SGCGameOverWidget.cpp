// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "UI/SGCGameOverWidget.h"
#include "Components/TextBlock.h"

void USGCGameOverWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();


    // Подписываемся на события смены состояния игры в GameMode 
    if (GetWorld())
    {
        const auto GameMode = Cast<ASGCGameMode>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnGameStateChanged.AddUObject(this, &USGCGameOverWidget::OnGameStateChanged);
        }
    }
}


// Вызывается при смене состояния игры
// Обрабатывает только состояния окончания игры
void USGCGameOverWidget::OnGameStateChanged(ESGCGameState State)
{
    if (State == ESGCGameState::EGS_GameOverWin)
    {
        WinText->SetVisibility(ESlateVisibility::Visible);
        LoseText->SetVisibility(ESlateVisibility::Hidden);
    }
    if (State == ESGCGameState::EGS_GameOverLose)
    {
        WinText->SetVisibility(ESlateVisibility::Hidden);
        LoseText->SetVisibility(ESlateVisibility::Visible);
    }
}
