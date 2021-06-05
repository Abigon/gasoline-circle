// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGCPlayerHUDWidget.generated.h"


/*
	Класс основного виджета игрока
	Выводит на экран уровень здоровья героя, кол-во монет в инвентаре,
	кол-во патронов в обойме и в инвентаре.
	В Blueprint добавлены в визуал виджеты информации о волнах, аукционе и обратном отсчете при запуске новой волны.
	Функции возвращают строковые и чмсловые значение. Все необходимые преобразования происходя в Blueprint
*/


UCLASS()
class GASOLINECIRCLE_API USGCPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	FString GetBulletsInfo() const;
	UFUNCTION(BlueprintCallable, Category = "UI")
	float GetHealthPercent() const;
	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetCoinsAmount() const;

protected:

	virtual void NativeOnInitialized() override;
	
	// Форматирует строку с кол-вом патронов так, чтобы она была всегда 3 символа
	// Недостающие символы заполняет нулями
	FString FormatBullets(int32 BulletsNum) const;

private:
	// Ссылка на ASGCMainCharacter, для сокращения кол-ва кастов
	class ASGCMainCharacter* PlayerPawn = nullptr;
};
