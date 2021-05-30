// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SGCMainMenuHUD.generated.h"


UCLASS()
class GASOLINECIRCLE_API ASGCMainMenuHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	virtual void BeginPlay() override;
};
