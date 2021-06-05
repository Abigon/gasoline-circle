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
	
	TotalWaves = WaveSpawnData.Num();
	CurrentWave = 0;

	SetGameState(ESGCGameState::EGS_InProgress);

	PlayerCharacter0 = Cast<ASGCMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	PlayerController0 = Cast<ASGCPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerCharacter0)
	{
		PlayerCharacter0->GetHealthComponent()->OnDeath.AddUObject(this, &ASGCGameMode::KillPlayer);
	}

	PrepareToWave();
}

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

void ASGCGameMode::KillEnemy()
{
	WaveLeftEnemies--;
	if (WaveLeftEnemies == 0) WaveOver();
}

void ASGCGameMode::KillPlayer()
{
	GameOver(false);
}

// Волны
void ASGCGameMode::StartWave()
{
	for (auto SpawnVolume : TActorRange<ASGCEnemySpawnVolume>(GetWorld()))
	{
		if (SpawnVolume) SpawnVolume->Reset();
	}

	if (PlayerController0)
	{
		PlayerController0->SetPlayerControlAvaible(true);
	}

	OnWaveStart.Broadcast();
	RestartSale();
	SpawnWave();
}

void ASGCGameMode::SpawnWave()
{
	
	if (CurrentWaveSpawnData.EnemiesData.Num() == 0) return;

	int32 MaxSpawn = ((CurrentWaveSpawnData.AmountEnemiesSpawnAtOnce != 0) && (CurrentWaveSpawnData.SecondsBetweenSpawn != 0)) ? CurrentWaveSpawnData.AmountEnemiesSpawnAtOnce : WaveLeftEnemies;

	for (int32 EnemyNum = 0; EnemyNum < MaxSpawn; EnemyNum++)
	{
		int32 EnemyClassIndex = (CurrentWaveSpawnData.EnemiesData.Num() > 1) ? FMath::RandRange(0, CurrentWaveSpawnData.EnemiesData.Num() - 1) : 0;
		
		auto SpawnVolume = GetEnemySpawnVolume();
		auto Enemy = SpawnVolume ? SpawnVolume->SpawnEnemy(CurrentWaveSpawnData.EnemiesData[EnemyClassIndex].EnemyClass) : nullptr;
		if (Enemy)
		{
			Enemy->GetHealthComponent()->OnDeath.AddUObject(this, &ASGCGameMode::KillEnemy);
			CurrentWaveSpawnData.EnemiesData[EnemyClassIndex].EnemiesAmount--;
		}

		if (CurrentWaveSpawnData.EnemiesData[EnemyClassIndex].EnemiesAmount == 0) CurrentWaveSpawnData.EnemiesData.RemoveAt(EnemyClassIndex);
		if (CurrentWaveSpawnData.EnemiesData.Num() == 0) break;
	}

	if (CurrentWaveSpawnData.EnemiesData.Num() > 0)
	{
		GetWorldTimerManager().SetTimer(WaveSpawnTimerHandle, this, &ASGCGameMode::SpawnWave, CurrentWaveSpawnData.SecondsBetweenSpawn, false);
	}
}

ASGCEnemySpawnVolume* ASGCGameMode::GetEnemySpawnVolume()
{
	if (!GetWorld()) return nullptr;

	bool IsBossWave = TotalWaves == (CurrentWave + 1);

	TArray<ASGCEnemySpawnVolume*> EnemySpawnsArray;

	for (auto EnemySpawnActor : TActorRange <ASGCEnemySpawnVolume>(GetWorld()))
	{
		if (EnemySpawnActor && EnemySpawnActor->IsCanSpawn() && (IsBossWave == EnemySpawnActor->IsCanSpawnBoss()))
		{
			EnemySpawnsArray.Add(EnemySpawnActor);
		}
	}

	int32 Index = FMath::RandRange(0, EnemySpawnsArray.Num() - 1);
	return EnemySpawnsArray[Index];
}

void ASGCGameMode::PrepareToWave()
{
	if (PlayerController0)
	{
		PlayerController0->SetPlayerControlAvaible(false);
	}

	CurrentWaveSpawnData = WaveSpawnData[CurrentWave];
	for (auto EnemyData : CurrentWaveSpawnData.EnemiesData)
	{
		WaveLeftEnemies += EnemyData.EnemiesAmount;
	}

	StopSale();
	OnStartWaveTimeCountdown.Broadcast();
	GetWorldTimerManager().SetTimer(WaveStartCountdownTimerHandle, this, &ASGCGameMode::StartWave, SecondsCountdownToWaveStart, false);
}

void ASGCGameMode::WaveOver()
{
	CurrentWave++;
	if (CurrentWave < TotalWaves)
	{
		StopWave(true);
		PrepareToWave();
	}
	else
	{
		GameOver(true);
	}
}

void ASGCGameMode::StopWave(bool IsResetCharacter)
{
	StopSale();
	GetWorldTimerManager().ClearAllTimersForObject(this);
	if (PlayerCharacter0)
	{
		PlayerCharacter0->GetWeaponComponent()->StopFire();
		if (IsResetCharacter) PlayerCharacter0->ResetPlayer(bPlayerHealthRestore, bPlayerBulletsReset, bPlayerCoinsReset);
	}

	for (auto Enemy : TActorRange<ASGCEnemy>(GetWorld()))
	{
		if (Enemy) Enemy->Destroy();
	}

	if (bRemoveCoinsFromMap)
	{
		for (auto Coin : TActorRange<ASGCCoin>(GetWorld()))
		{
			if (Coin) Coin->Destroy();
		}
	}
}

bool ASGCGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);

	if (PauseSet)
	{
		SetGameState(ESGCGameState::EGS_Pause);
	}
	return PauseSet;
}

bool ASGCGameMode::ClearPause()
{
	const auto PauseCleared = Super::ClearPause();
	if (PauseCleared)
	{
		SetGameState(ESGCGameState::EGS_InProgress);
	}
	return PauseCleared;
}

void ASGCGameMode::SetGameState(ESGCGameState State)
{
	if (CurrentGameState == State) return;

	CurrentGameState = State;
	OnGameStateChanged.Broadcast(CurrentGameState);
}

void ASGCGameMode::CheckLevel()
{
	checkf(WaveSpawnData.Num() > 0, TEXT("Count of Waves must be Above ZERO!!!!"));

	TArray<AActor*> EnemySpawnsArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASGCEnemySpawnVolume::StaticClass(), EnemySpawnsArray);

	checkf(EnemySpawnsArray.Num() > 0, TEXT("SpawnVolumes must be Above ZERO!!!!"));
	
	int32 CountEnemiesSpawning = 0;	
	for (auto EnemySpawn : EnemySpawnsArray)
	{
		auto SpawnVolume = Cast<ASGCEnemySpawnVolume>(EnemySpawn);
		CountEnemiesSpawning += SpawnVolume->GetSpawningCount();
	}
	
	int32 MaxEnemiesInWave = 0;
	for (auto Wave : WaveSpawnData)
	{
		int32 TempEmeniesAmount = 0;
		for (auto EnemyData : Wave.EnemiesData)
		{
			TempEmeniesAmount += EnemyData.EnemiesAmount;
			if (!EnemyData.EnemyClass)
			{
				UE_LOG(LogTemp, Error, TEXT("Enemy Class Must Be Setup"));
			}
		}
		MaxEnemiesInWave = TempEmeniesAmount > MaxEnemiesInWave ? TempEmeniesAmount : MaxEnemiesInWave;
	}

	if (MaxEnemiesInWave > CountEnemiesSpawning)
	{
		UE_LOG(LogTemp, Error, TEXT("Max Enemies Amount In One Wave MORE Than EnemySpawnVolumes In World Can Spawn Enemies!!!!!"));
	}
}

// Аукцион на продажу патронов
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

void ASGCGameMode::EndSale()
{
	StopSale();
	RestartSale();
}

void ASGCGameMode::RestartSale()
{
	int32 SecondsToNextSale = FMath::RandRange(SecondsToSaleMin, SecondsToSaleMax);
	GetWorldTimerManager().SetTimer(NextSaleTimerHandle, this, &ASGCGameMode::StartSale, SecondsToNextSale, false);
}

void ASGCGameMode::StopSale()
{
	bIsSale = false;
	GetWorldTimerManager().ClearTimer(SaleCountdownTimerHandle);
	OnFinishBulletsSale.Broadcast();
}

void ASGCGameMode::SetCurrentPriceOfBullets()
{
	CurrentPriceOfBullets -= StepPriceOfCountdown;
	if (CurrentPriceOfBullets == 0)
	{
		GetWorldTimerManager().ClearTimer(SaleCountdownTimerHandle);
	}
}

