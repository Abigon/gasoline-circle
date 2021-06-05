// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGCGoToMainMenuWidget.generated.h"


/*
	Класс кнопки перехода в главное меню
	Кнопка создана чтобы не дублировать код в нескольких виджетах
*/


UCLASS()
class GASOLINECIRCLE_API USGCGoToMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// ССылки на Button в Blueprint для связи 
	UPROPERTY(meta = (BindWidget))
	class UButton* GoToMainMenuButton;

	virtual void NativeOnInitialized() override;

private:
	// Вызывается при нажатии на кнопку
	UFUNCTION()
	void OnGoToMainMenu();
};
