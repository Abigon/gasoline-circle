// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SGCMainMenuHUD.generated.h"


/*
	Класс HUD главного меню.
	Нужен для корректной устновки в GameMode главного меню
	Отвечает за вывод виджета главного меню, создавая и выводя его при загрузке уровня
	Ссылка на класс виджета устанавливается в Blueprint
	Класс можно расширить для выбора уровня на котором играть, если добавить несколько уровней в игру
*/

UCLASS()
class GASOLINECIRCLE_API ASGCMainMenuHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	virtual void BeginPlay() override;
};
