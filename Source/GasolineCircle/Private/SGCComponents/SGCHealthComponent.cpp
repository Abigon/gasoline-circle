// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "SGCComponents/SGCHealthComponent.h"

USGCHealthComponent::USGCHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USGCHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxHealth > 0);
	SetHealth(MaxHealth);

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USGCHealthComponent::OnTakeAnyDamage);
	}
}

void USGCHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	ApplyDamage(Damage);
}

void USGCHealthComponent::SetHealth(float NewHealth)
{
	const auto NextHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);
	Health = NextHealth;
}

void USGCHealthComponent::ApplyDamage(float Damage)
{
	if (Damage <= 0.0f || IsDead()) return;

	SetHealth(Health - Damage);

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
}
