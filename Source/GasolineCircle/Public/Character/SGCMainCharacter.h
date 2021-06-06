// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SGCMainCharacter.generated.h"


/*
	Класс игрока.
	Чарактер с анимацией.
	Управление происходит вперед/назад и стрэйф вправо/влево.
	Поворот осуществляется мышью. Карта стоит на месте.
	Обрабатывает управление игроком, покупку патроновна аукционе, сбор монет
	Кол-во жизни игрока устанавливается в HealthComponent.
	Скорость персонажа устанавливается в CharacterMovementComponent.
	Оружие выбирается в WeaponComponent.
*/

UCLASS()
class GASOLINECIRCLE_API ASGCMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	// Геттер для виджета
	int32 GetCoinAmount() const { return CoinAmount; }

	// Добавление монет при луте
	void AddCoins(int32 Coins);

	// Востановление начальных параметров персонажа перед началом новой волны
	void ResetPlayer(bool bIsRestoreHealth, bool bIsResetAmmo, bool bIsResetCoins);

	ASGCMainCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE class USGCWeaponComponent* GetWeaponComponent() const { return WeaponComponent; }
	FORCEINLINE class USGCHealthComponent* GetHealthComponent() const { return HealthComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class USGCWeaponComponent* WeaponComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class USGCHealthComponent* HealthComponent;

	// Скорость вращения персонажа. Используется с Геймпадами
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rotate")
	float BaseTurnRate = 45.f;

	// Параметры для зума камеры. В задании не требовалось
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zoom")
	float CameraZoomMax = 3000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zoom")
	float CameraZoomMin = 600.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zoom")
	float CameraZoomSpeed = 200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zoom")
	float CameraZoomDefault = 1600.f;

	// Звуки для успешной и неудачной покупки на аукционе
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
	class USoundCue* SaleSuccessSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
	class USoundCue* SaleErrorSound;

	// Маскимально возможное кол-во монет в интентаре
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coin", meta = (ToolTip = "If equal to 0 then unlimited"))
	int32 MaxCoinAmount = 0.f;

	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void CameraZoomIn();
	void CameraZoomOut();
	void RotateToCursor();

	void OnDeath();

private:
	// Текущее кол-во монет 
	int32 CoinAmount = 0;

	class APlayerController* CharacterConstroller = nullptr;

	// Функции для работы с аукционом
	bool BuyBullets(int32 Coins, int32 Bullets);
	void TryBuyBullets();

	// Проверка возможности потратить заданное ко-во монет
	bool CanPay(int32 Price) const { return CoinAmount >= Price; }
};
