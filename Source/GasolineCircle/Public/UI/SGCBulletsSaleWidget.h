// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGCBulletsSaleWidget.generated.h"


/*
	Класс виджета для отображения акциона по продаже патронов
	Функции возвращают численные значение. Все необходимые преобразования происходя в Blueprint

*/


UCLASS()
class GASOLINECIRCLE_API USGCBulletsSaleWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// Возвращает кол-во патронов на аукционе
	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetBulletsForSale() const { return BulletsAmount; }

	// Возвращает текущюю цену аукциона
	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetPriceOfBullets() const;

protected:

	// Кол-во патронов на аукционе. Инициализируется 1 раз при начале аукциона, чтобы сократить кол-во запросов
	int32 BulletsAmount = 0;

	virtual void NativeOnInitialized() override;

	void OnStartSale(int32 Bullets);
	void OnFinishSale();

private:
	// Ссылка на GameMode, для сокращения кол-ва кастов
	class ASGCGameMode* GameMode = nullptr;
};
