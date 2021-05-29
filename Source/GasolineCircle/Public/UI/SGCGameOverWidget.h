// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/SGCGameMode.h"
#include "SGCGameOverWidget.generated.h"


UCLASS()
class GASOLINECIRCLE_API USGCGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeOnInitialized() override;

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WinText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LoseText;

private:
	void OnGameStateChanged(ESGCGameState State);
};
