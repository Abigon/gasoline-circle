// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGCGameMode.generated.h"

UENUM(Blueprinttype)
enum class ESGCGameState : uint8
{
	EGS_Waiting UMETA(DisplayName = "Game Waiting To Start"),
	EGS_InProgress UMETA(DisplayName = "Game In Progress"),
	EGS_Pause UMETA(DisplayName = "Pause Game"),
	EGS_GameOverWin UMETA(DisplayName = "Win The Game"),
	EGS_GameOverLose UMETA(DisplayName = "Lose The Game "),
	EGS_MAX UMETA(DisplayName = "DefaultMAX")
};

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
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedSignature, ESGCGameState);
DECLARE_MULTICAST_DELEGATE(FOnStartWaveTimeCountdownSignature);
DECLARE_MULTICAST_DELEGATE(FOnWaveStartSignature);

UCLASS()
class GASOLINECIRCLE_API ASGCGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	ASGCGameMode();

	FOnStartBulletsSaleSignature OnStartBulletsSale;
	FOnFinishBulletsSaleSignature OnFinishBulletsSale;
	FOnGameStateChangedSignature OnGameStateChanged;
	FOnStartWaveTimeCountdownSignature OnStartWaveTimeCountdown;
	FOnWaveStartSignature OnWaveStart;

	virtual void StartPlay() override;
	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
	virtual bool ClearPause() override;

	bool IsSale() const { return bIsSale; }
	int32 GetCurrentPriceOfBullets() const { return CurrentPriceOfBullets; }
	int32 GetBulletsForSale() const { return BulletsForSale; }
	void EndSale();

	int32 GetCurrentWave() const { return CurrentWave + 1; }
	int32 GetTotalWaves() const { return TotalWaves; }

	float GetWaveStartCountdownTimer() const { return GetWorldTimerManager().GetTimerRemaining(WaveStartCountdownTimerHandle); }
	int32 GetWaveLeftEnemies() const { return WaveLeftEnemies; }

	void KillEnemy();
	void KillPlayer();


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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
	class USoundCue* SaleStartSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave Data")
	TArray<FWaveSpawnData> WaveSpawnData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave Data")
	float SecondsCountdownToWaveStart = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "On New Wave")
	bool bPlayerBulletsReset = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "On New Wave")
	bool bPlayerHealthRestore = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "On New Wave")
	bool bPlayerCoinsReset = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "On New Wave")
	bool bRemoveCoinsFromMap = true;

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

	ESGCGameState CurrentGameState = ESGCGameState::EGS_Waiting;

	class ASGCMainCharacter* PlayerCharacter0 = nullptr;
	class ASGCPlayerController* PlayerController0 = nullptr;

	void StartSale();
	void StopSale();
	void RestartSale();
	void SetCurrentPriceOfBullets();

	void GameOver(bool bIsWin);
	void WaveOver();
	void StartWave();
	void StopWave(bool IsResetCharacter);
	void PrepareToWave();

	void SpawnWave();
	class ASGCEnemySpawnVolume* GetEnemySpawnVolume();
	void CheckLevel();

	void SetGameState(ESGCGameState State);
};



/*
Старт игры
- заблокировать управление
- подписаться на обновление смерти перса
- запустить состояние и таймер до новой волны


Старт новой волны
- сбросить точки респа врагов
- разблокировать управление 
- сгенерировать первую волну мобов
- запустить таймер на следующую волну
- запустить таймер аукциона


Окончание волны при убийстве всех мобов
- окончить аукцион
- остановить все таймеры
- остановить стрельбу перса
- заблокировать управление персом
- удалить или нет монеты с карты
- сбросить состояние перса
- запустить состояние и таймер до новой волны


Победа
- окончить аукцион
- остановить все таймеры
- остановить стрельбу перса
- заблокировать управление персом??? или не надо т.к. будет виджет
- поменять состояние игры на ПОБЕДУ


Смерть персонажа
- остановить стрельбу перса
- окончить аукцион
- остановить все таймеры
- удалить всех врагов с карты
- заблокировать управление персом??? или не надо т.к. будет виджет
- поменять состояние игры на ПРОИГРЫШ





переход в меню
??? хз загружается другой уровень


управление персом
+++ монеты на карте
+++ Состояние перса: монеты, патроны
таймер аукциона
состояние стрельбы перса
враги на карте


*/