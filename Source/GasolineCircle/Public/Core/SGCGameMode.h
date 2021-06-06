// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGCGameMode.generated.h"


/*
	Класс основного игрового GameMode.
	Обрабатывает игровой цикл, изменяет состояние игры, генерирует волны мобов, 
	Запускает и контролирует аукцион на монеты.

	Позволяет гибко настраивать кол-во волн, типы и количество мобов в волне
	Точка спауна босса активируется только на посленней волне.
	Позволяет настраивать параметры для сброса состояния играка перед новой волной:
	восстанавливать или нет здоровье, патроны и списывать или нет монеты перед новой волной.

	При запуске игры происходит проверка на соответствие кол-ва мобов в волнах и 
	кол-ва возможных спаунов на всех точках спауна врагов. При несоответствии выдаст ошибку
*/


// Перечисление состояний игры
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

// Структура для определения Типа и кол-ва мобов для спауна в волне
USTRUCT(BlueprintType)
struct FEnemySpawnData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave Data")
	TSubclassOf<class ASGCEnemy> EnemyClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave Data")
	int32 EnemiesAmount;
};

// Структура для описания волны. Содержит массив с типом/кол-вом мобов
// Кол-во мобов, которые спаунятся одновременно и время между спаунами мобов.
// Если хотя бы один из двух параметров равен 0, то заспавнятся все мобы волны сразу при старте
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

// Делегат старта аукциона на патроны
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStartBulletsSaleSignature, int32);

// Делегат окончания аукциона на патроны
DECLARE_MULTICAST_DELEGATE(FOnFinishBulletsSaleSignature);

// Делегат смены состояния игры
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedSignature, ESGCGameState);

// Делегат начала отсчета перед стартом новый волны
DECLARE_MULTICAST_DELEGATE(FOnStartWaveTimeCountdownSignature);

// Делегат старта новой волны и окончания отсчета
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

	// Возвращает состояние аукциона
	bool IsSale() const { return bIsSale; }

	// Интерфесные функции аукциона для виджетов
	int32 GetCurrentPriceOfBullets() const { return CurrentPriceOfBullets; }
	int32 GetBulletsForSale() const { return BulletsForSale; }

	// окончание аукциона
	void EndSale();

	// Интерфесные фнукции для виджета
	int32 GetCurrentWave() const { return CurrentWave + 1; }
	int32 GetTotalWaves() const { return TotalWaves; }
	int32 GetWaveLeftEnemies() const { return WaveLeftEnemies; }
	float GetWaveStartCountdownTimer() const { return GetWorldTimerManager().GetTimerRemaining(WaveStartCountdownTimerHandle); }

	// Обработка смерти мобов и игрока
	void KillEnemy();
	void KillPlayer();


protected:
	// Настройки аукциона на патроны
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullets Sale", meta = (ToolTip = "Min value in seconds before the start of a new sale"))
	int32 SecondsToSaleMin = 5;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullets Sale", meta = (ToolTip = "Max value in seconds before the start of a new sale"))
	int32 SecondsToSaleMax = 10;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullets Sale", meta = (ToolTip = "Min count of bullets at sale"))
	int32 BulletsForSaleMin = 20;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullets Sale", meta = (ToolTip = "Max count of bullets at sale"))
	int32 BulletsForSaleMax = 40;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullets Sale", meta = (ToolTip = "Min price of bullets at sale"))
	int32 PriceOfBulletsMin = 20;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullets Sale", meta = (ToolTip = "Max price of bullets at sale"))
	int32 PriceOfBulletsMax = 40;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullets Sale", meta = (ToolTip = "Step in seconds to lower the price at sale"))
	float StepSecondsOfCountdown = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullets Sale", meta = (ToolTip = "Price Step to lower the price at sale"))
	int32 StepPriceOfCountdown = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
	class USoundCue* SaleStartSound;

	// Настройка волн
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave Data", meta = (ToolTip = "Must be at least one wave"))
	TArray<FWaveSpawnData> WaveSpawnData;

	// Время обратного отсчета перед стартом новой волны
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave Data", meta = (ToolTip = "Countdown time before the start of a new wave"))
	float SecondsCountdownToWaveStart = 5.f;

	// Настройки восстановления состяния перед новой волной
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "On New Wave", meta = (ToolTip = "Restore the player's bullets to default before the start of a new wave"))
	bool bPlayerBulletsReset = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "On New Wave", meta = (ToolTip = "Restore the player's health to default before the start of a new wave"))
	bool bPlayerHealthRestore = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "On New Wave", meta = (ToolTip = "Reset player's coins before the start of a new wave"))
	bool bPlayerCoinsReset = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "On New Wave", meta = (ToolTip = "Remove coins from map before the start of a new wave"))
	bool bRemoveCoinsFromMap = true;

private:
	// Таймеры
	FTimerHandle NextSaleTimerHandle;
	FTimerHandle SaleCountdownTimerHandle;
	FTimerHandle WaveStartCountdownTimerHandle;
	FTimerHandle WaveSpawnTimerHandle;

	//Текущее состояние аукциона
	bool bIsSale = false;
	int32 CurrentPriceOfBullets = 0;
	int32 BulletsForSale = 0;

	// Текущий номер волня
	int32 CurrentWave = 0;
	
	// Общее кол-во волн
	int32 TotalWaves = 0;

	// Кол-во врагов до окончания волны
	int32 WaveLeftEnemies = 0;

	// Информация о текущей волне
	FWaveSpawnData CurrentWaveSpawnData;

	// Текущее состояние
	ESGCGameState CurrentGameState = ESGCGameState::EGS_Waiting;

	// Ссылки для сокращения кастов
	class ASGCMainCharacter* PlayerCharacter0 = nullptr;
	class ASGCPlayerController* PlayerController0 = nullptr;

	// Обработка аукциона
	void StartSale();
	void StopSale();
	void RestartSale();
	void SetCurrentPriceOfBullets();

	// Обработка основного игрового цикла
	void GameOver(bool bIsWin);
	void WaveOver();
	void StartWave();
	void StopWave(bool IsResetCharacter);
	void PrepareToWave();
	void SpawnWave();
	class ASGCEnemySpawnVolume* GetEnemySpawnVolume();

	// Проверка уровня перед стартом игры
	void CheckLevel();

	// Установка текущего состояния игры
	void SetGameState(ESGCGameState State);
};
