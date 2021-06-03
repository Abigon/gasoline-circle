// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "UI/SGCGoToMainMenuWidget.h"
#include "Core/SGCGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void USGCGoToMainMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GoToMainMenuButton)
    {
        GoToMainMenuButton->OnClicked.AddDynamic(this, &USGCGoToMainMenuWidget::OnGoToMainMenu);
    }
}

void USGCGoToMainMenuWidget::OnGoToMainMenu()
{
    if (!GetWorld()) return;
    const auto SGCGameInstance = GetWorld()->GetGameInstance<USGCGameInstance>();
    if (!SGCGameInstance) return;

    if (SGCGameInstance->GetMainMenuLevelName().IsNone())
    {
        UE_LOG(LogTemp, Error, TEXT("Main Menu level name is NONE"));
        return;
    }
    UGameplayStatics::OpenLevel(this, SGCGameInstance->GetMainMenuLevelName());
}

