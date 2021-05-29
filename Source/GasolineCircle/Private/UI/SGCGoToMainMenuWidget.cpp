// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "UI/SGCGoToMainMenuWidget.h"
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
    //if (!GetWorld()) return;

    //const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
    //if (!STUGameInstance) return;

    //if (STUGameInstance->GetMenuLevelName().IsNone())
    //{
    //    UE_LOG(LogTemp, Error, TEXT("Menu level name is NONE"));
    //    return;
    //}
    //UGameplayStatics::OpenLevel(this, STUGameInstance->GetMenuLevelName());
}

