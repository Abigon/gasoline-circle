// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "UI/SGCPlayerHUDWidget.h"
#include "SGCComponents/SGCWeaponComponent.h"
#include "SGCComponents/SGCHealthComponent.h"

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
	const auto PlayerPawn = GetOwningPlayerPawn();
	if (!PlayerPawn) return "0 / 0";
	const auto WeaponComponent = Cast<USGCWeaponComponent>(PlayerPawn->GetComponentByClass(USGCWeaponComponent::StaticClass()));
	if (!WeaponComponent) return "0 / 0";

	return FormatBullets(WeaponComponent->GetCurrentBulletsInClip()) + " / " + FormatBullets(WeaponComponent->GetCurrentTotalBullets());
}

float USGCPlayerHUDWidget::GetHealthPercent() const
{
	const auto PlayerPawn = GetOwningPlayerPawn();
	if (!PlayerPawn) return 0.f;
	const auto HealthComponent = Cast<USGCHealthComponent>(PlayerPawn->GetComponentByClass(USGCHealthComponent::StaticClass()));
	if (!HealthComponent) return 0.f;

	return HealthComponent->GetHealthPercent();
}

int32 USGCPlayerHUDWidget::GetCoinsAmount() const
{
	//const auto PlayerPawn = GetOwningPlayerPawn();
	//if (!PlayerPawn) return "0 / 0";
	//const auto WeaponComponent = Cast<USGCWeaponComponent>(PlayerPawn->GetComponentByClass(USGCWeaponComponent::StaticClass()));
	//if (!WeaponComponent) return "0 / 0";

	return 5;
}
