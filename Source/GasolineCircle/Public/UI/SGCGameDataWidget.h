// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGCGameDataWidget.generated.h"


UCLASS()
class GASOLINECIRCLE_API USGCGameDataWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	FString GetWaveLeftEnemies() const;
	UFUNCTION(BlueprintCallable, Category = "UI")
	FString GetWaveInfo() const;
};
