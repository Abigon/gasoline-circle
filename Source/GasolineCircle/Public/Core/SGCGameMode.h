// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGCGameMode.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnStartBulletsSaleSignature, int32);
DECLARE_MULTICAST_DELEGATE(FOnFinishBulletsSaleSignature);

UCLASS()
class GASOLINECIRCLE_API ASGCGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	ASGCGameMode();

	FOnStartBulletsSaleSignature OnStartBulletsSale;
	FOnFinishBulletsSaleSignature OnFinishBulletsSale;

	virtual void StartPlay() override;
	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
	virtual bool ClearPause() override;

	bool IsSale() const { return bIsSale; }
	int32 GetCurrentPriceOfBullets() const { return CurrentPriceOfBullets; }
	int32 GetBulletsForSale() const { return BulletsForSale; }
	void EndSale();

	float GetWaveTimerRate() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sale")
	int32 SecondsToSaleMin = 5;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sale")
	int32 SecondsToSaleMax = 10;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sale")
	int32 BulletsForSaleMin = 20;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sale")
	int32 BulletsForSaleMax = 40;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sale")
	int32 PriceOfBulletsMin = 20;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sale")
	int32 PriceOfBulletsMax = 40;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sale")
	float StepSecondsOfCountdown = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sale")
	int32 StepPriceOfCountdown = 1.f;

private:
//	FTimerHandle GameWaveTimerHandle;
	FTimerHandle NextSaleTimerHandle;
	FTimerHandle SaleCountdownTimerHandle;

	bool bIsSale = false;
	int32 CurrentPriceOfBullets = 0;
	int32 BulletsForSale = 0;

	void StartSale();
	void SetCurrentPriceOfBullets();

	void GameOver();
	void WaveOver();
	void StartWave();
};
