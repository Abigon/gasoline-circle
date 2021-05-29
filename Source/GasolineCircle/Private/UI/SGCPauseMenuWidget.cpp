// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "Core/SGCGameMode.h"
#include "UI/SGCPauseMenuWidget.h"
#include "Components/Button.h"

void USGCPauseMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ContinueButton)
    {
        ContinueButton->OnClicked.AddDynamic(this, &USGCPauseMenuWidget::OnContinueGame);
    }
}

void USGCPauseMenuWidget::OnContinueGame()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;
    GetWorld()->GetAuthGameMode()->ClearPause();
}