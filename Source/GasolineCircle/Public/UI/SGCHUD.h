// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SGCHUD.generated.h"


UCLASS()
class GASOLINECIRCLE_API ASGCHUD : public AHUD
{
	GENERATED_BODY()
	
public:

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UUserWidget* CurrentWidget = nullptr;
};
