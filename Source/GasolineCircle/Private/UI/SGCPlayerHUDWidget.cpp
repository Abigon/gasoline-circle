// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "Character/SGCMainCharacter.h"
#include "SGCComponents/SGCWeaponComponent.h"
#include "SGCComponents/SGCHealthComponent.h"
#include "UI/SGCBulletsSaleWidget.h"
#include "UI/SGCPlayerHUDWidget.h"

void USGCPlayerHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//if (GetOwningPlayer())
	//{
	//	GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &USTUPlayerHUDWidget::OnNewPawn);
	//	OnNewPawn(GetOwningPlayerPawn());
	//}
}

FString USGCPlayerHUDWidget::FormatBullets(int32 BulletsNum) const
{
	const int32 MaxLength = 3;
	const TCHAR PrefixSymbol = '0';

	auto BulletStr = FString::FromInt(BulletsNum);
	const auto SymbolsNumToAdd = MaxLength - BulletStr.Len();

	if (SymbolsNumToAdd > 0)
	{
		BulletStr = FString::ChrN(SymbolsNumToAdd, PrefixSymbol).Append(BulletStr);
	}

	return BulletStr;
}

FString USGCPlayerHUDWidget::GetBulletsInfo() const
{
	const auto PlayerPawn = Cast<ASGCMainCharacter>(GetOwningPlayerPawn());
	if (!PlayerPawn) return "0 / 0";

	return FormatBullets(PlayerPawn->GetWeaponComponent()->GetCurrentBulletsInClip()) + " / " + FormatBullets(PlayerPawn->GetWeaponComponent()->GetCurrentTotalBullets());
}

float USGCPlayerHUDWidget::GetHealthPercent() const
{
	const auto PlayerPawn = Cast<ASGCMainCharacter>(GetOwningPlayerPawn());
	if (!PlayerPawn) return 0.f;

	return PlayerPawn->GetHealthComponent()->GetHealthPercent();
}

int32 USGCPlayerHUDWidget::GetCoinsAmount() const
{
	const auto PlayerPawn = Cast<ASGCMainCharacter>(GetOwningPlayerPawn());
	if (!PlayerPawn) return 0;

	return PlayerPawn->GetCoinAmount();
}
