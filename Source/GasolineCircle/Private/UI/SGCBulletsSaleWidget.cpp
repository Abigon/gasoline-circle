// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "UI/SGCBulletsSaleWidget.h"
#include "Core/SGCGameMode.h"

void USGCBulletsSaleWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetVisibility(ESlateVisibility::Hidden);

	// Инициализируем GameMode
	if (!GetWorld()) return;
	GameMode = Cast<ASGCGameMode>(GetWorld()->GetAuthGameMode());

	// Подписываемся на события начала и окончания аукциона в GameMode 
	if (GameMode)
	{
		GameMode->OnStartBulletsSale.AddUObject(this, &USGCBulletsSaleWidget::OnStartSale);
		GameMode->OnFinishBulletsSale.AddUObject(this, &USGCBulletsSaleWidget::OnFinishSale);
	}
}


// Начало аукциона. Инициализируем кол-во патронов в предложении и отображаем виджет в HUD
void USGCBulletsSaleWidget::OnStartSale(int32 Bullets)
{
	BulletsAmount = Bullets;
	SetVisibility(ESlateVisibility::Visible);
}

// Скрываем виджет при окончании аукциона
void USGCBulletsSaleWidget::OnFinishSale()
{
	SetVisibility(ESlateVisibility::Hidden);
}


// Возвращаем текущую цену предложения
int32 USGCBulletsSaleWidget::GetPriceOfBullets() const
{
	// Если виджет скрыт, запрос не отрабатываем
	if (!IsVisible()) return 0;

	return GameMode ? GameMode->GetCurrentPriceOfBullets() : 0;
}

