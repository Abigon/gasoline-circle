// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SGCHealthComponent.generated.h"

/*
	Класс компонента здоровья.
	Используется героем и врагами.
	Реализует инициализацию здоровья и получение урона. 
	Сообшает о событии при смерти персонажа.
	В реализации задач по данной игре можно обойтись без этого класса и перенести весь функционал в Character или Врага
*/


DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASOLINECIRCLE_API USGCHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	FOnDeathSignature OnDeath;
	
	USGCHealthComponent();

	// Проверка на смерть персонажа
	bool IsDead() const { return FMath::IsNearlyZero(Health); }

	// Геттер для виджета
	float GetHealthPercent() const { return Health / MaxHealth; }

	// Сбрасывает здоровье до перноначальное состояния
	void RestoreHealth() { SetHealth(MaxHealth); }

protected:
	virtual void BeginPlay() override;

	// Максимальные уровень здоровья.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0", ClampMax = "500.0"))
	float MaxHealth = 100.f;

	// Класс для тряски камеры при попадании на мину. 
	// Если не установлен тряски не будет
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effect")
	TSubclassOf<class UCameraShakeBase> CameraShake;

private:
	// Текущий уровень здоровья
	float Health;

	// Получение урона по событию
	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void SetHealth(float NewHealth);
	void ApplyDamage(float Damage);
	void PlayCameraShake();
};
