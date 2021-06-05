// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGCPauseMenuWidget.generated.h"


/*
	Класс виджета меню паузы
	Используется 2 кнопки с установленным функционалом 
	и только одна кнопка продолжения игры 
*/


UCLASS()
class GASOLINECIRCLE_API USGCPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeOnInitialized() override;

protected:
	// ССылки на Button в Blueprint для связи 
	UPROPERTY(meta = (BindWidget))
	class UButton* ContinueButton;

private:
	UFUNCTION()
	void OnContinueGame();
};

