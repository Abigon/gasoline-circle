// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Core/SGCGameMode.h"
#include "SGCHUD.generated.h"


UCLASS()
class GASOLINECIRCLE_API ASGCHUD : public AHUD
{
	GENERATED_BODY()
	
public:

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PauseGameClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	virtual void BeginPlay() override;

private:
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
