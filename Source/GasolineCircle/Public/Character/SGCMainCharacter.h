// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SGCMainCharacter.generated.h"

UCLASS()
class GASOLINECIRCLE_API ASGCMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	bool CanPay(int32 Price) const { return CoinAmount >= Price; }
	int32 GetCoinAmount() const { return CoinAmount; }
	void AddCoins(int32 Coins);
	void PainCoin(int32 Coins);


	ASGCMainCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE class USGCWeaponComponent* GetWeaponComponent() const { return WeaponComponent; }
	FORCEINLINE class USGCHealthComponent* GetHealthComponent() const { return HealthComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class USGCWeaponComponent* WeaponComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class USGCHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera | Rotate")
	float BaseTurnRate = 45.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera | Zoom")
	float CameraZoomMax = 3000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera | Zoom")
	float CameraZoomMin = 600.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera | Zoom")
	float CameraZoomSpeed = 200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera | Zoom")
	float CameraZoomDefault = 1600.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coin")
	int32 MaxCoinAmount = 0.f;

	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void CameraZoomIn();
	void CameraZoomOut();

	void OnDeath();
private:

	int32 CoinAmount = 0;

};
