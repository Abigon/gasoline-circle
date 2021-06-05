// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SGCWeaponComponent.generated.h"


/*
	Класс компонента оружия для героя.
	Можно расширить для использования нескольких видов оружия (создать массив с оружием и добавив функции по смене оружия.
	Является интерфесом между персонажем и коркретным оружием.
	В реализации задач по данной игре можно обойтись без этого класса и перенести весь функционал в Character

*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASOLINECIRCLE_API USGCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USGCWeaponComponent();

	virtual void StartFire();
	void StopFire();
	void Reload();
	void ResetDefaultAmmo();
	void AddCurrentBullets(int32 Bullets);


	// Геттеры для вижджета
	int32 GetCurrentTotalBullets() const;
	int32 GetCurrentBulletsInClip() const;

protected:
	// Класс оружия для спауна при старте игры. Выбирается в Blueprint
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class ASGCWeapon> WeaponClass;

	// Название сокета на скелете персонажа для аттача оружия
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName WeaponSocketName = "WeaponSocket";

	// Текущеее выбранное оружие
	UPROPERTY()
	ASGCWeapon* CurrentWeapon = nullptr;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void SpawnWeapon();		
};
