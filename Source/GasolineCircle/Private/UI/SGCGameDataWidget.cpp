// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "UI/SGCGameDataWidget.h"
#include "Core/SGCGameMode.h"

int32 USGCGameDataWidget::GetWaveSeconds() const
{
	return GetWorld() ? GetWorld()->GetTimeSeconds() : 0;
}

FString USGCGameDataWidget::GetWaveInfo() const
{
	const auto World = Cast<ASGCGameMode>(GetWorld()->GetAuthGameMode());
	if (!World)	return "0 / 0";
	return FString::Printf(TEXT("Wave %i / %i"), World->GetCurrentWave(), World->GetTotalWaves());
}

FString USGCGameDataWidget::GetWaveLeftEnemies() const
{
	const auto World = Cast<ASGCGameMode>(GetWorld()->GetAuthGameMode());
	if (!World)	return "Enemies: 0+";
	return FString::Printf(TEXT("Enemies: %i"), World->GetWaveLeftEnemies());
}