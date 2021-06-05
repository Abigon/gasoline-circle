// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGCMainMenuWidget.generated.h"


/*
	Класс основного виджета главного меню
	Выводит на экран 2 кнопки - старт игры и вывод из игры,
	В Blueprint добавлена информация об управлении и прочие ассеты.
*/


UCLASS()
class GASOLINECIRCLE_API USGCMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// ССылки на Button в Blueprint для связи 
	UPROPERTY(meta = (BindWidget))
	class UButton* StartGameButton;
	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameButton;

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnStartGame();

	UFUNCTION()
	void OnQuitGame();
};
