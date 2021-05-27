// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGCGameMode.generated.h"


USTRUCT(BlueprintType)
struct FEnemySpawnData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave Data")
	TSubclassOf<class ASGCEnemy> EnemyClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave Data")
	int32 EnemiesAmount;
};

USTRUCT(BlueprintType)
struct FWaveSpawnData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave Data")
	TArray<FEnemySpawnData> EnemiesData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave Data")
	float SecondsBetweenSpawn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave Data")
	int32 AmountEnemiesSpawnAtOnce;
};

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

	int32 GetCurrentWave() const { return CurrentWave + 1; }
	int32 GetTotalWaves() const { return TotalWaves; }
	int32 GetWaveLeftEnemies() const { return WaveLeftEnemies; }

	void KillEnemy();


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullets Sale")
	int32 SecondsToSaleMin = 5;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullets Sale")
	int32 SecondsToSaleMax = 10;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullets Sale")
	int32 BulletsForSaleMin = 20;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullets Sale")
	int32 BulletsForSaleMax = 40;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullets Sale")
	int32 PriceOfBulletsMin = 20;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullets Sale")
	int32 PriceOfBulletsMax = 40;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullets Sale")
	float StepSecondsOfCountdown = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullets Sale")
	int32 StepPriceOfCountdown = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave Data")
	TArray<FWaveSpawnData> WaveSpawnData;

private:
	FTimerHandle NextSaleTimerHandle;
	FTimerHandle SaleCountdownTimerHandle;
	FTimerHandle WaveStartCountdownTimerHandle;
	FTimerHandle WaveSpawnTimerHandle;

	bool bIsSale = false;
	int32 CurrentPriceOfBullets = 0;
	int32 BulletsForSale = 0;

	int32 CurrentWave = 0;
	int32 TotalWaves = 0;
	int32 WaveLeftEnemies = 0;

	FWaveSpawnData CurrentWaveSpawnData;

	void StartSale();
	void SetCurrentPriceOfBullets();

	void GameOver();
	void WaveOver();
	void StartWave();

	void SpawnWave();
	class ASGCEnemySpawnVolume* GetEnemySpawnVolume();
	void CheckLevel();


};
