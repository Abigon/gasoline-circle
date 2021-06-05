// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Items/SGCItem.h"
#include "Weapon/SGCExplosiveDamageType.h"
#include "SGCExplosive.generated.h"


/*
	Класс мины.
	Взорваться на мине могут классы, выбранные в TSet.
	Таким образом, можно менять на кого воздействует мина, без изменения кода, а только настройками в Blueprint
	Минус данной реализации, что взрываться будет только конкретный класс, его наследники будут прогнарированы.
	Мина не удаляется с карты, а появляется снова через настроенное время
*/


UCLASS()
class GASOLINECIRCLE_API ASGCExplosive : public ASGCItem
{
	GENERATED_BODY()

public:
	virtual void OnCollisionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

protected:
	// Урон от взрыва
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosive")
	float DamageAmount = 3.f;

	// Тип урона. В данной реализации нужен только для проигрывания тряски камеры
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosive")
	TSubclassOf<UDamageType> SGCDamageType = USGCExplosiveDamageType::StaticClass();

	// Время появления мины на карте снова после взрыва
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosive")
	float SecondsToRespawn = 5.f;

	// Набор классов, которые могут взрываться на мине. настраивается в Blueprint. 
	// При пустом сете, все будут игнорировать мину
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosive")
	TSet <TSubclassOf<APawn>> DamagedPawnsClasses;

	virtual void BeginPlay() override;

private:
	// Таймер восстановления мины
	FTimerHandle RespawnTimerHandle;

	// Функция восстановления мины
	void Respawn();

};

