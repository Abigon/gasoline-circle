// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/SGCItem.h"
#include "SGCCoin.generated.h"

/*
	Класс монет.
	Может лежать на карте или спавнится мобами
	Берется только героем
*/

UCLASS()
class GASOLINECIRCLE_API ASGCCoin : public ASGCItem
{
	GENERATED_BODY()
	
public:	

	// Устанавливает кол-во монет в луте
	void SetAmount(int32 NewAmount) { Amount = NewAmount; }

	// Переопределенная функция для обработки перекрятия объекта
	virtual void OnCollisionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

protected:
	// Кол-во монет в луте
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coin")
	int32 Amount = 1;

};
