// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SGCEnemy.generated.h"

/*
	Класс моба (врага).
	Моб повернут к игроку всегда лицом и движется к нему.
	Мобы могут друг в друга упираться.
	Для передвижения добавлен UFloatingPawnMovement.
	Это позволяет добавлять на карту объекты, которые моб будет обходить, в отличии от функции, которая будет протсто двигать моба к игроку.
	Минусом этого компонета является отсутствие гравитации. Поэтому при спауне мобы остаются на той высоте, на которой сгенерировались.
	Для нанесения урона есть несколько параметров, настраиваемых в Blueprint:
	уровень урона, внешний радиус поражения и внутренний, время между атаками. Если внутренний равен 0, то фактически урон наносится по площади внешнего радиуса.
	Если внутренний радиус больше нуля, то урон нанесется объекту, находящемуся между внутренним и внешним радиусом
	Мобы могут спавнить монеты, для это есть соответствующие настройки
	Размер моба настраивается во Viewport в Blueprint. При этом надо не забыть правильно установить меш в 0-точку
	Скорость передвижения моба настраивается в Blueprint в MovementComponent, параметр MaxSpeed
	Максимальный уровень жизни настраивается в HealthComponent
	При появлении моба и смерти воспроизводится звук. При смерти также воспроизводится эффект
	Анимация моба сделана через прыжки. Можно отключить и настроить.

*/

UCLASS()
class GASOLINECIRCLE_API ASGCEnemy : public APawn
{
	GENERATED_BODY()
	
public:	
	ASGCEnemy();
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class USGCHealthComponent* GetHealthComponent() const { return HealthComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UBoxComponent* CollisionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class USGCHealthComponent* HealthComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UFloatingPawnMovement* MovementComponent;
	
	// Уровень урона
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float Damage = 1;

	// Размер внешнего радиуса урона
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float OuterDamageRadius = 150.f;

	// Рахмер внутреннего радиуса урона
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float InnerDamageRadius = 0.f;

	// Время между атаками моба
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float TimeBetweenDamage = 1.0f;

	// Отображать сферы урона или нет. Работает только при отладке. В релизной версии отображаться не будут
	// В идеале заменить эффектами
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	bool bShowDamegeSphere = true;

	// Звуки и эффекты
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	class UParticleSystem* DeathParticles;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
	class USoundCue* SpawnSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
	class USoundCue* DeathSound;


	// Парамерты для спауна монет

	// Класс монеты для спауна
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coins Spawn")
	TSubclassOf<class ASGCCoin> CoinsClass;

	// Время между попятком спауна монет
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coins Spawn")
	float TimeBetweenCoinsSpawn = 3.0f;

	// Шанс спауна монет
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coins Spawn", meta = (ClampMin = "0", ClampMax = "100.0"))
	float ChanceToCoinsSpawnPercent = 30.0f;

	// Минимальное кол-во монет для спауна
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coins Spawn")
	int32 CoinsSpawnMin = 5;

	// Максимальное кол-во монет для спауна
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coins Spawn")
	int32 CoinsSpawnMax = 10;

	// Анимация моба
	
	// Прыгает моб или нет
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coins Animation")
	bool bEnemyJump = true;

	// Высота прыжка
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coins Animation", meta = (EditCondition = "bEnemyJump"))
	float JumpHeight = 90.0f;

	// Скорость прыжка
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coins Animation", meta = (EditCondition = "bEnemyJump"))
	float JumpVelocity = 180.0f;

	virtual void BeginPlay() override;

private:

	// Ссылка на героя, чтобы сократить кол-во кастов
	class ASGCMainCharacter* PlayerPawn = nullptr;

	// Таймер между уронами
	FTimerHandle DamageTimerHandle;

	// Таймер между попытками спавна монет
	FTimerHandle CoinSpawnHandle;

	// Переменные для реализазии прыжков
	float StartZ = 0;
	float MaxJumpZ = 0;
	bool bIsMoveUp = true;

	void ApplyDamage();
	void OnDeath();
	void MoveToPlayer();
	void RotateToPlayer();
	void EnemyJump(float DeltaTime);
	void SpawnCoins();
};
