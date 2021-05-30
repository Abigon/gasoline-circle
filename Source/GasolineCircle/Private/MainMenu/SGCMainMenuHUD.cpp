// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "MainMenu/SGCMainMenuHUD.h"
#include "MainMenu/SGCMainMenuWidget.h"


void ASGCMainMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    if (MainMenuWidgetClass)
    {
        auto MenuWidget = CreateWidget<USGCMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
        if (MenuWidget)
        {
            MenuWidget->AddToViewport();
            MenuWidget->SetVisibility(ESlateVisibility::Visible);
        }
    }
}
