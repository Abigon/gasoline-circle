// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SGCGameInstance.generated.h"


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
