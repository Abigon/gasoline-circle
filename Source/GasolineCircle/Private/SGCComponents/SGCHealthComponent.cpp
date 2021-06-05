// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "SGCComponents/SGCHealthComponent.h"
#include "Weapon/SGCExplosiveDamageType.h"


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

	if (DamageType->IsA<USGCExplosiveDamageType>())
	{
		PlayCameraShake();
	}
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

void USGCHealthComponent::PlayCameraShake()
{
	if (!CameraShake) return;

	const auto Player = Cast<APawn>(GetOwner());
	if (!Player) return;

	const auto Controller = Player->GetController<APlayerController>();
	if (!Controller || !Controller->PlayerCameraManager) return;

	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}
