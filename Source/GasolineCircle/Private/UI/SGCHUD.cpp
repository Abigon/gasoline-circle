// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "UI/SGCHUD.h"
#include "Blueprint/UserWidget.h"


void ASGCHUD::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerHUDWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
		CurrentWidget->AddToViewport();
		CurrentWidget->SetVisibility(ESlateVisibility::Visible);
	}

}
