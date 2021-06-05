// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "UI/SGCGameDataWidget.h"
#include "Core/SGCGameMode.h"


void USGCGameDataWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Инициализируем GameMode
	if (!GetWorld()) return;
	GameMode = Cast<ASGCGameMode>(GetWorld()->GetAuthGameMode());
}

// Возвращает строку с текущей волной и общим кол-ом волн
FString USGCGameDataWidget::GetWaveInfo() const
{
	if (!GameMode)	return "0 / 0";
	return FString::Printf(TEXT("Wave %i / %i"), GameMode->GetCurrentWave(), GameMode->GetTotalWaves());
}

// Возвращает строку с кол-вом оставшихся врагов
FString USGCGameDataWidget::GetWaveLeftEnemies() const
{
	if (!GameMode)	return "Enemies: 0+";
	return FString::Printf(TEXT("Enemies: %i"), GameMode->GetWaveLeftEnemies());
}