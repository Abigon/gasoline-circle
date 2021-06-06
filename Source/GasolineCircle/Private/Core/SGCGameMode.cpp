// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SGCGameMode.h"
#include "Core/SGCPlayerController.h"
#include "Character/SGCMainCharacter.h"
#include "Enemy/SGCEnemy.h"
#include "Enemy/SGCEnemySpawnVolume.h"
#include "Items/SGCCoin.h"
#include "SGCComponents/SGCHealthComponent.h"
#include "SGCComponents/SGCWeaponComponent.h"
#include "UI/SGCHUD.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Sound/SoundCue.h"


ASGCGameMode::ASGCGameMode()
{
	DefaultPawnClass = ASGCMainCharacter::StaticClass();
	PlayerControllerClass = ASGCPlayerController::StaticClass();
	HUDClass = ASGCHUD::StaticClass();
}

void ASGCGameMode::StartPlay()
{
	Super::StartPlay();

	CheckLevel();
	
	// Инициализация волн
	TotalWaves = WaveSpawnData.Num();
	CurrentWave = 0;

	SetGameState(ESGCGameState::EGS_InProgress);

	PlayerCharacter0 = Cast<ASGCMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	PlayerController0 = Cast<ASGCPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerCharacter0)
	{
		// Подписываемся на смерть героя для обработки проигрыша
		PlayerCharacter0->GetHealthComponent()->OnDeath.AddUObject(this, &ASGCGameMode::KillPlayer);
	}

	PrepareToWave();
}

// Окончание игры. Выводим состояние победы или поражение
void ASGCGameMode::GameOver(bool bIsWin)
{
	StopWave(false);

	if (bIsWin)
	{
		UE_LOG(LogTemp, Warning, TEXT("Win"));
		SetGameState(ESGCGameState::EGS_GameOverWin);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Lose"));
		SetGameState(ESGCGameState::EGS_GameOverLose);
	}
}

// Смерть врага. Уменьшаем кол-во оставшихся врагов
// Если враги кончились, то волна оканчивается
void ASGCGameMode::KillEnemy()
{
	WaveLeftEnemies--;
	if (WaveLeftEnemies == 0) WaveOver();
}

// При смерти персонажа игра оканчивается поражением
void ASGCGameMode::KillPlayer()
{
	GameOver(false);
}

// Обработка Волн

// Подготовка к началу волны
void ASGCGameMode::PrepareToWave()
{
	// Юлокируем управление игрока
	if (PlayerController0)
	{
		PlayerController0->SetPlayerControlAvaible(false);
	}

	// Заполняем информацию о новой вволне
	CurrentWaveSpawnData = WaveSpawnData[CurrentWave];
	for (auto EnemyData : CurrentWaveSpawnData.EnemiesData)
	{
		WaveLeftEnemies += EnemyData.EnemiesAmount;
	}

	// Сбрасываем аукцион и запускаем обратный отсчет
	StopSale();
	OnStartWaveTimeCountdown.Broadcast();
	GetWorldTimerManager().SetTimer(WaveStartCountdownTimerHandle, this, &ASGCGameMode::StartWave, SecondsCountdownToWaveStart, false);
}

// Старт волны
void ASGCGameMode::StartWave()
{
	// Восстанавливаем точки спауна
	for (auto SpawnVolume : TActorRange<ASGCEnemySpawnVolume>(GetWorld()))
	{
		if (SpawnVolume) SpawnVolume->Reset();
	}

	// Разрешаем игроку управлять персонажем
	if (PlayerController0)
	{
		PlayerController0->SetPlayerControlAvaible(true);
	}

	// Сообщаем о старте новой волны
	OnWaveStart.Broadcast();
	
	// Запускаем отсчет до аукциона
	RestartSale();

	// Начинаем спаунить врагов
	SpawnWave();
}

void ASGCGameMode::SpawnWave()
{
	if (CurrentWaveSpawnData.EnemiesData.Num() == 0) return;

	//Определяем кол-во врагов для спауна за 1 раз
	int32 MaxSpawn = ((CurrentWaveSpawnData.AmountEnemiesSpawnAtOnce != 0) && (CurrentWaveSpawnData.SecondsBetweenSpawn != 0)) ? CurrentWaveSpawnData.AmountEnemiesSpawnAtOnce : WaveLeftEnemies;

	// Спауним врагов
	for (int32 EnemyNum = 0; EnemyNum < MaxSpawn; EnemyNum++)
	{
		// Если в волне несколько видов врагов, то выбираем какой спаунить
		int32 EnemyClassIndex = (CurrentWaveSpawnData.EnemiesData.Num() > 1) ? FMath::RandRange(0, CurrentWaveSpawnData.EnemiesData.Num() - 1) : 0;
		
		auto SpawnVolume = GetEnemySpawnVolume();
		if (SpawnVolume)
		{
			auto Enemy = SpawnVolume->SpawnEnemy(CurrentWaveSpawnData.EnemiesData[EnemyClassIndex].EnemyClass);
			if (Enemy)
			{
				Enemy->GetHealthComponent()->OnDeath.AddUObject(this, &ASGCGameMode::KillEnemy);
				CurrentWaveSpawnData.EnemiesData[EnemyClassIndex].EnemiesAmount--;
			}
		}
		// Если враги определенного типа кончились, удаляем информацию о них
		if (CurrentWaveSpawnData.EnemiesData[EnemyClassIndex].EnemiesAmount == 0) CurrentWaveSpawnData.EnemiesData.RemoveAt(EnemyClassIndex);
		
		// Если врагов в волне больше нет прекращаем цикл спауна
		if (CurrentWaveSpawnData.EnemiesData.Num() == 0) break;
	}

	// Если еще нужно спаунить врагов, запускаем таймер до следующей волны
	if (CurrentWaveSpawnData.EnemiesData.Num() > 0)
	{
		GetWorldTimerManager().SetTimer(WaveSpawnTimerHandle, this, &ASGCGameMode::SpawnWave, CurrentWaveSpawnData.SecondsBetweenSpawn, false);
	}
}

// Возвращаем случайную точку для спауна
ASGCEnemySpawnVolume* ASGCGameMode::GetEnemySpawnVolume()
{
	if (!GetWorld()) return nullptr;

	bool IsBossWave = TotalWaves == (CurrentWave + 1);

	TArray<ASGCEnemySpawnVolume*> EnemySpawnsArray;
	EnemySpawnsArray.Empty();

	// Составляем список доступных точек спауна
	for (auto EnemySpawnActor : TActorRange <ASGCEnemySpawnVolume>(GetWorld()))
	{
		if (EnemySpawnActor && EnemySpawnActor->IsCanSpawn() && (IsBossWave == EnemySpawnActor->IsCanSpawnBoss()))
		{
			EnemySpawnsArray.Add(EnemySpawnActor);
		}
	}

	//Возвращаем рандомную
	int32 Index = FMath::RandRange(0, EnemySpawnsArray.Num() - 1);
	return EnemySpawnsArray[Index];
}

// Окончание волный
void ASGCGameMode::WaveOver()
{
	CurrentWave++;
	if (CurrentWave < TotalWaves)
	{
		// Если волны еще есть, то сбрасываем текущую и начинаем новую
		StopWave(true);
		PrepareToWave();
	}
	else
	{
		// Иначе победа
		GameOver(true);
	}
}

// Сброс волны
void ASGCGameMode::StopWave(bool IsResetCharacter)
{
	// Останавливаем аукцион и сбрасываем все таймеры
	StopSale();
	GetWorldTimerManager().ClearTimer(NextSaleTimerHandle);
	//GetWorldTimerManager().ClearAllTimersForObject(this);

	// Останавливаем стрельбу и сбрасываем параметры игрока
	if (PlayerCharacter0)
	{
		PlayerCharacter0->GetWeaponComponent()->StopFire();
		if (IsResetCharacter) PlayerCharacter0->ResetPlayer(bPlayerHealthRestore, bPlayerBulletsReset, bPlayerCoinsReset);
	}

	if (!GetWorld()) return;

	// Удаляем всех врагов с карты
	for (auto Enemy : TActorRange<ASGCEnemy>(GetWorld()))
	{
		if (Enemy) Enemy->Destroy();
	}

	// Удаляем монеты с карты
	if (bRemoveCoinsFromMap)
	{
		for (auto Coin : TActorRange<ASGCCoin>(GetWorld()))
		{
			if (Coin) Coin->Destroy();
		}
	}
}

// Установка паузы
bool ASGCGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);

	if (PauseSet)
	{
		SetGameState(ESGCGameState::EGS_Pause);
	}
	return PauseSet;
}

// Снятие паузы
bool ASGCGameMode::ClearPause()
{
	const auto PauseCleared = Super::ClearPause();
	if (PauseCleared)
	{
		SetGameState(ESGCGameState::EGS_InProgress);
	}
	return PauseCleared;
}

// Установка текущего состояния
void ASGCGameMode::SetGameState(ESGCGameState State)
{
	if (CurrentGameState == State) return;

	CurrentGameState = State;
	OnGameStateChanged.Broadcast(CurrentGameState);
}

// Аукцион на продажу патронов

// Начало аукциона
void ASGCGameMode::StartSale()
{
	bIsSale = true;
	GetWorldTimerManager().ClearTimer(NextSaleTimerHandle);

	CurrentPriceOfBullets = FMath::RandBool() ? PriceOfBulletsMax : PriceOfBulletsMin;
	BulletsForSale = FMath::RandBool() ? BulletsForSaleMax : BulletsForSaleMin;

	OnStartBulletsSale.Broadcast(BulletsForSale);
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SaleStartSound);
	}
	GetWorldTimerManager().SetTimer(SaleCountdownTimerHandle, this, &ASGCGameMode::SetCurrentPriceOfBullets, StepSecondsOfCountdown, true);
}

// Окончание аукциона. Останавливаем текущий запускаем отсчет до нового
void ASGCGameMode::EndSale()
{
	StopSale();
	RestartSale();
}

// Запускаем тамер до нового аукциона
void ASGCGameMode::RestartSale()
{
	int32 SecondsToNextSale = FMath::RandRange(SecondsToSaleMin, SecondsToSaleMax);
	GetWorldTimerManager().SetTimer(NextSaleTimerHandle, this, &ASGCGameMode::StartSale, SecondsToNextSale, false);
}

// Остановка текущего аукциона
void ASGCGameMode::StopSale()
{
	bIsSale = false;
	GetWorldTimerManager().ClearTimer(SaleCountdownTimerHandle);
	OnFinishBulletsSale.Broadcast();
}

// Установка новой цены на аукционе 
void ASGCGameMode::SetCurrentPriceOfBullets()
{
	CurrentPriceOfBullets -= StepPriceOfCountdown;
	if (CurrentPriceOfBullets <= 0)
	{
		GetWorldTimerManager().ClearTimer(SaleCountdownTimerHandle);
	}
}


// Проверка настроек волн и уровня на SpawnVolume
void ASGCGameMode::CheckLevel()
{
	bool bLevelIsOk = true;

	if (WaveSpawnData.Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Count of Waves must be Above ZERO!!!!"));
		bLevelIsOk = false;
	}

	int32 MaxEnemiesInWave = 0;
	for (auto Wave : WaveSpawnData)
	{
		if (Wave.EnemiesData.Num() <= 0)
		{
			UE_LOG(LogTemp, Error, TEXT("Count of Enemy's Types in wave must be Above ZERO i!!!!"));
			bLevelIsOk = false;
		}
		int32 TempEmeniesAmount = 0;
		for (auto EnemyData : Wave.EnemiesData)
		{
			if (!EnemyData.EnemyClass)
			{
				UE_LOG(LogTemp, Error, TEXT("Enemy Class Must Be Setup"));
			}
			if (EnemyData.EnemiesAmount <=0)
			{
				UE_LOG(LogTemp, Error, TEXT("Count of Enemies in wave must be Above ZERO i!!!!"));
				bLevelIsOk = false;
			}
			TempEmeniesAmount += EnemyData.EnemiesAmount;
		}
		MaxEnemiesInWave = TempEmeniesAmount > MaxEnemiesInWave ? TempEmeniesAmount : MaxEnemiesInWave;
	}

	TArray<AActor*> EnemySpawnsArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASGCEnemySpawnVolume::StaticClass(), EnemySpawnsArray);
	
	if (EnemySpawnsArray.Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnVolumes on map must be Above ZERO!!!!"));
		bLevelIsOk = false;
	}

	bool bIsBossSpawnVolume = false;
	int32 CountEnemiesSpawning = 0;
	for (auto EnemySpawn : EnemySpawnsArray)
	{
		auto SpawnVolume = Cast<ASGCEnemySpawnVolume>(EnemySpawn);
		CountEnemiesSpawning += SpawnVolume->GetSpawningCount();
		if (SpawnVolume->IsCanSpawnBoss()) bIsBossSpawnVolume = true;
	}

	if (MaxEnemiesInWave > CountEnemiesSpawning)
	{
		UE_LOG(LogTemp, Error, TEXT("Max Enemies Amount In One Wave MORE Than EnemySpawnVolumes In World Can Spawn Enemies!!!!!"));
		bLevelIsOk = false;
	}

	if (!bIsBossSpawnVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnVolumes for BOSS must be present on map!!!!"));
		bLevelIsOk = false;
	}

	checkf(bLevelIsOk, TEXT("Have some errors on map. See LOG!!"));
}

