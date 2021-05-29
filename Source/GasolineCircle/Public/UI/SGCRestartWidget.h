// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGCRestartWidget.generated.h"


UCLASS()
class GASOLINECIRCLE_API USGCRestartWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* RestartButton;

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnRestart();
};
