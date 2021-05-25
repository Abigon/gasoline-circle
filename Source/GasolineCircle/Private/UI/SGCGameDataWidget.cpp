// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "UI/SGCGameDataWidget.h"
#include "Core/SGCGameMode.h"

int32 USGCGameDataWidget::GetWaveSeconds() const
{
	if (!GetWorld()) return 0;
	//auto GameMode = Cast<ASGCGameMode>(GetWorld()->GetAuthGameMode());

	return GetWorld()->GetTimeSeconds();//GameMode ? GameMode->GetWaveTimerRate() : 0;
}