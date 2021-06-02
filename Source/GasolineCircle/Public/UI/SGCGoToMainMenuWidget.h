// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGCGoToMainMenuWidget.generated.h"


UCLASS()
class GASOLINECIRCLE_API USGCGoToMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* GoToMainMenuButton;

	virtual void NativeOnInitialized() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FName MenuLevelName = NAME_None;

	UFUNCTION()
	void OnGoToMainMenu();
};
