// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGCPauseMenuWidget.generated.h"


UCLASS()
class GASOLINECIRCLE_API USGCPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeOnInitialized() override;

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* ContinueButton;

private:
	UFUNCTION()
	void OnContinueGame();
};

