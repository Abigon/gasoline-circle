// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "UI/SGCPauseMenuWidget.h"
#include "Core/SGCGameMode.h"
#include "Components/Button.h"

void USGCPauseMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ContinueButton)
    {
        ContinueButton->OnClicked.AddDynamic(this, &USGCPauseMenuWidget::OnContinueGame);
    }
}


// При нажатии кномпки снимаем паузу в GameMode
void USGCPauseMenuWidget::OnContinueGame()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;
    GetWorld()->GetAuthGameMode()->ClearPause();
}