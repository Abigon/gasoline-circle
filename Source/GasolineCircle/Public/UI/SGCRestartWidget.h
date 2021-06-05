// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGCRestartWidget.generated.h"


/*
	Класс кнопки перезапука уровня
	Кнопка создана чтобы не дублировать код в нескольких виджетах
*/


UCLASS()
class GASOLINECIRCLE_API USGCRestartWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// ССылки на Button в Blueprint для связи 
	UPROPERTY(meta = (BindWidget))
	class UButton* RestartButton;

	virtual void NativeOnInitialized() override;

private:
	// Вызывается при нажатии на кнопку
	UFUNCTION()
	void OnRestart();
};
