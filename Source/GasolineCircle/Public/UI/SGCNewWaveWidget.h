// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGCNewWaveWidget.generated.h"


/*
	Класс виджета для отображения обратного отсчета перед стартом новой волны мобов,
	Является частью виджета основного HUD игрока
	Функции возвращают строковые значение. Все необходимые преобразования происходя в Blueprint
*/


UCLASS()
class GASOLINECIRCLE_API USGCNewWaveWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	FString GetLeftSeconds() const;

protected:

	virtual void NativeOnInitialized() override;

	void OnStartCountdown();
	void OnFinishCountdown();

private:
	// Ссылка на GameMode, для сокращения кол-ва кастов
	class ASGCGameMode* GameMode = nullptr;
};
