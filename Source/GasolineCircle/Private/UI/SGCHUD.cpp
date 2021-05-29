// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "UI/SGCHUD.h"
#include "Blueprint/UserWidget.h"


void ASGCHUD::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerHUDWidgetClass)
	{
		PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
		PlayerHUDWidget->AddToViewport();
		PlayerHUDWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (PauseGameClass)
	{
		PauseWidget = CreateWidget<UUserWidget>(GetWorld(), PauseGameClass);
		PauseWidget->AddToViewport();
		PauseWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (GameOverWidgetClass)
	{
		GameOvertWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
		GameOvertWidget->AddToViewport();
		GameOvertWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (GetWorld())
	{
		if (const auto GameMode = Cast<ASGCGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->OnGameStateChanged.AddUObject(this, &ASGCHUD::OnGameStateChanged);
		}
	}
}

void ASGCHUD::OnGameStateChanged(ESGCGameState State)
{
	if (CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	switch (State)
	{
	case ESGCGameState::EGS_InProgress:
		CurrentWidget = PlayerHUDWidget; 
		break;
	case ESGCGameState::EGS_Pause:
		CurrentWidget = PauseWidget;
		break;
	case ESGCGameState::EGS_GameOverWin:
	case ESGCGameState::EGS_GameOverLose:
		CurrentWidget = GameOvertWidget;
		break;
	default:
		CurrentWidget = nullptr;
	}

	if (CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
