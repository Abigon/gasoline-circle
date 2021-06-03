// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGCMainMenuWidget.generated.h"


UCLASS()
class GASOLINECIRCLE_API USGCMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
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
