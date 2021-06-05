// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SGCGameInstance.generated.h"


/*
	Класс GameInstance. 
	Нужен для хранения наименований уровня игры и уровня главного меню
	Моэно расширить под несколько уровней игры с соответствующими изменениями в MainMenu
*/


UCLASS()
class GASOLINECIRCLE_API USGCGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	FName GetStartLevelName() const { return StartLevelName; }
	FName GetMainMenuLevelName() const { return MenuLevelName; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FName MenuLevelName = NAME_None;
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FName StartLevelName = NAME_None;
};
