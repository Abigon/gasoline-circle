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

	ASGCMainCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class USGCWeaponComponent* WeaponComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class USGCHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float BaseTurnRate = 45.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera Zoom")
	float CameraZoomMax = 3000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera Zoom")
	float CameraZoomMin = 600.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera Zoom")
	float CameraZoomSpeed = 200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera Zoom")
	float CameraZoomDefault = 1600.f;

	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void CameraZoomIn();
	void CameraZoomOut();

	void OnDeath();
private:

};
