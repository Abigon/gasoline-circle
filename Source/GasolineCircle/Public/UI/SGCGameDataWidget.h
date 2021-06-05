// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGCGameDataWidget.generated.h"


/*
	Класс виджета для отображения номера текущей волны, 
	общего кол-ва волн и оставшегося кол-ва врагов.
	Является частью виджета основного HUD игрока
	Функции возвращают строковые значение. Все необходимые преобразования происходя в Blueprint
*/


UCLASS()
class GASOLINECIRCLE_API USGCGameDataWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Возвращает строку с кол-вом оставшихся врагов
	UFUNCTION(BlueprintCallable, Category = "UI")
	FString GetWaveLeftEnemies() const;

	// Возвращает строку с текущей волной и общим кол-ом волн
	UFUNCTION(BlueprintCallable, Category = "UI")
	FString GetWaveInfo() const;


protected:

	virtual void NativeOnInitialized() override;

private:
	// Ссылка на GameMode, для сокращения кол-ва кастов
	class ASGCGameMode* GameMode = nullptr;
};
