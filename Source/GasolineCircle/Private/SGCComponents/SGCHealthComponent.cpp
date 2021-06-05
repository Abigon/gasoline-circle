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

	// Подписываемся на событие получения урона владельцем компонента
	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USGCHealthComponent::OnTakeAnyDamage);
	}
}

// Получение урона по событию
void USGCHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	// Применяем урон
	ApplyDamage(Damage);

	// Если урон от мины, то проигрываем тряску камеры
	if (DamageType->IsA<USGCExplosiveDamageType>())
	{
		PlayCameraShake();
	}
}

// Устанавливаем уровень здоровья не больше максимально возможного
void USGCHealthComponent::SetHealth(float NewHealth)
{
	const auto NextHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);
	Health = NextHealth;
}

// Применяем урон.
// Если смерть, то сообщаем об этом подписчикам
void USGCHealthComponent::ApplyDamage(float Damage)
{
	if (Damage <= 0.0f || IsDead()) return;

	SetHealth(Health - Damage);

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
}

// Проигрываем тряску камеры
// При смерти или не выбранном классе эффекта тряски не будет
void USGCHealthComponent::PlayCameraShake()
{
	if (IsDead() || !CameraShake) return;

	const auto Player = Cast<APawn>(GetOwner());
	if (!Player) return;

	const auto Controller = Player->GetController<APlayerController>();
	if (!Controller || !Controller->PlayerCameraManager) return;

	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}
