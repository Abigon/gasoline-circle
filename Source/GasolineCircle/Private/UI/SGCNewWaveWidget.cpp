// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "UI/SGCNewWaveWidget.h"
#include "Core/SGCGameMode.h"



void USGCNewWaveWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetVisibility(ESlateVisibility::Hidden);

	// Инициализируем GameMode
	if (!GetWorld()) return;
	GameMode = Cast<ASGCGameMode>(GetWorld()->GetAuthGameMode());

	// Подписываемся на события начала обратного отсчет и его окончания
	if (GameMode)
	{
		GameMode->OnStartWaveTimeCountdown.AddUObject(this, &USGCNewWaveWidget::OnStartCountdown);
		GameMode->OnWaveStart.AddUObject(this, &USGCNewWaveWidget::OnFinishCountdown);
	}
}

// Показываем виджет при начале отсчета
void USGCNewWaveWidget::OnStartCountdown()
{
	SetVisibility(ESlateVisibility::Visible);
}

// Прячем виджет при окончании отсчета
void USGCNewWaveWidget::OnFinishCountdown()
{
	SetVisibility(ESlateVisibility::Hidden);
}


// Возыращаем цифру обратного остчета или Go!
FString USGCNewWaveWidget::GetLeftSeconds() const
{
	if (!IsVisible()) return "";

	int32 TimeToWave = GameMode ? GameMode->GetWaveStartCountdownTimer() : 0;
	if (TimeToWave < 1) return "Go!";
	return FString::FromInt(TimeToWave);
}

