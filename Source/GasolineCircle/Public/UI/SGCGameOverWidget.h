// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/SGCGameMode.h"
#include "SGCGameOverWidget.generated.h"


/*
	Класс виджета окончания игры
	В зависимости от победы или проигрыша выводится соответствующая строка, а вторая скрывается

*/


UCLASS()
class GASOLINECIRCLE_API USGCGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeOnInitialized() override;

protected:
	// ССылки на TextBlock в Blueprint для связи 
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WinText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LoseText;

private:
	// Вызывается при смене состояния игры
	void OnGameStateChanged(ESGCGameState State);
};
