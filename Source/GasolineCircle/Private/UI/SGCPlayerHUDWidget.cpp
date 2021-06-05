// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "UI/SGCPlayerHUDWidget.h"
#include "Character/SGCMainCharacter.h"
#include "SGCComponents/SGCWeaponComponent.h"
#include "SGCComponents/SGCHealthComponent.h"


void USGCPlayerHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerPawn = Cast<ASGCMainCharacter>(GetOwningPlayerPawn());
}


// Форматирует строку с кол-вом патронов так, чтобы она была всегда 3 символа
// Недостающие символы заполняет нулями
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
	if (!PlayerPawn) return "0 / 0";
	return FormatBullets(PlayerPawn->GetWeaponComponent()->GetCurrentBulletsInClip()) + " / " + FormatBullets(PlayerPawn->GetWeaponComponent()->GetCurrentTotalBullets());
}

float USGCPlayerHUDWidget::GetHealthPercent() const
{
	if (!PlayerPawn) return 0.f;
	return PlayerPawn->GetHealthComponent()->GetHealthPercent();
}

int32 USGCPlayerHUDWidget::GetCoinsAmount() const
{
	if (!PlayerPawn) return 0;
	return PlayerPawn->GetCoinAmount();
}
