// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Core/SGCGameMode.h"
#include "SGCHUD.generated.h"


/*
	Класс основного HUD в игре. Назначается в GameMode
	Отвечает за вывод корректрого виджета в зависимости от состояния игры
*/


UCLASS()
class GASOLINECIRCLE_API ASGCHUD : public AHUD
{
	GENERATED_BODY()
	
public:

protected:

	// Классы на виджеты основного HUD игрока, паузы и окончания игры
	// Устанавливаются в Blueprint
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PauseGameClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	virtual void BeginPlay() override;

private:
	// Сслыки на созданные виджеты и сслыка на текущий установленный виджет
	UPROPERTY()
	UUserWidget* CurrentWidget = nullptr;
	UPROPERTY()
	UUserWidget* PauseWidget = nullptr;
	UPROPERTY()
	UUserWidget* PlayerHUDWidget = nullptr;
	UPROPERTY()
	UUserWidget* GameOvertWidget = nullptr;

	void OnGameStateChanged(ESGCGameState State);
};
