// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "UI/SGCBulletsSaleWidget.h"
#include "Core/SGCGameMode.h"

void USGCBulletsSaleWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetVisibility(ESlateVisibility::Hidden);

	if (!GetWorld()) return;
	auto GameMode = Cast<ASGCGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode) 
	{
		GameMode->OnStartBulletsSale.AddUObject(this, &USGCBulletsSaleWidget::OnStartSale);
		GameMode->OnFinishBulletsSale.AddUObject(this, &USGCBulletsSaleWidget::OnFinishSale);
	}
}

void USGCBulletsSaleWidget::OnStartSale(int32 Bullets)
{
	BulletsAmount = Bullets;
	SetVisibility(ESlateVisibility::Visible);
}

void USGCBulletsSaleWidget::OnFinishSale()
{
	SetVisibility(ESlateVisibility::Hidden);
}

int32 USGCBulletsSaleWidget::GetPriceOfBullets() const
{
	if (!GetWorld()) return 0;

	auto GameMode = Cast<ASGCGameMode>(GetWorld()->GetAuthGameMode());
	return GameMode ? GameMode->GetCurrentPriceOfBullets() : 0;
}

