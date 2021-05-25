// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGCBulletsSaleWidget.generated.h"


UCLASS()
class GASOLINECIRCLE_API USGCBulletsSaleWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetBulletsForSale() const { return BulletsAmount; }
	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetPriceOfBullets() const;

protected:
	int32 BulletsAmount = 0;

	virtual void NativeOnInitialized() override;

	void OnStartSale(int32 Bullets);
	void OnFinishSale();

private:

};
