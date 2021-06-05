// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGCWeapon.generated.h"


/*
	Класс оружия. Можно сделать шаблоном для других видов оружия при расширении
	Не поддерживает автоматическую перезарядку при пустом магазине. Только ручная.
*/


UCLASS()
class GASOLINECIRCLE_API ASGCWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ASGCWeapon();

	void StartFire();
	void StopFire();
	void Reload();
	void ResetAmmo();
	void AddBullets(int32 NewBullets);

	bool IsClipEmpty() const { return BulletsInClip == 0; }
	bool IsAmmoEmpty() const { return TotalBullets == 0; }


	// Геттеры для UI
	int32 GetTotalBullets() const { return TotalBullets; }
	int32 GetBulletsInClip() const { return BulletsInClip; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class USkeletalMeshComponent* WeaponMesh;

	// Класс патрона 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class ASGCProjectile>ProjectileClass;

	// Сокет дула, для определения стартовой точки и направления патрона
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName MuzzleSocketName = "Muzzle";

	// Время медлу выстрелами при зажатой клавише стрельбы
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TimeBetweenShots = 0.5f;

	// Максимальное кол-во патронов в инвентаре
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ToolTip = "Max bullets in inventory"))
	int32 MaxBullets = 90;

	// Емкость патронов в магазине
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ToolTip = "Max bullets in one clip"))
	int32 MaxBulletsInClip = 15;

	// Стартовое кол-во патронов в инвентаре
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ToolTip = "Bullets in inventory on game's start"))
	int32 StartBullets = 60;


	// Ссылки на звуки
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
	class USoundCue* ShotSound;	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
	class USoundCue* ReloadSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds", meta = (ToolTip = "Sound for shoot when clip is empty"))
	class USoundCue* EmptySound;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void MakeShot();

private:

	// Таймер беспрерывной стрельбы
	FTimerHandle ShotTimerHandle;

	// Текущее кол-во патронов в инвентаре
	int32 TotalBullets = 0;

	// Текущее кол-во патронов в магазине
	int32 BulletsInClip = 0;

};
