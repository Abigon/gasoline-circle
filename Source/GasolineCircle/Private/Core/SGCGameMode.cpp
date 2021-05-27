// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SGCGameMode.h"
#include "Core/SGCPlayerController.h"
#include "Character/SGCMainCharacter.h"
#include "Enemy/SGCEnemy.h"
#include "Enemy/SGCEnemySpawnVolume.h"
#include "SGCComponents/SGCHealthComponent.h"
#include "UI/SGCHUD.h"
#include "Kismet/GameplayStatics.h"


ASGCGameMode::ASGCGameMode()
{
	DefaultPawnClass = ASGCMainCharacter::StaticClass();
	PlayerControllerClass = ASGCPlayerController::StaticClass();
	HUDClass = ASGCHUD::StaticClass();
//	PlayerStateClass = ASTUPlayerState::StaticClass();
}

void ASGCGameMode::StartPlay()
{
	Super::StartPlay();

	CheckLevel();
	
	TotalWaves = WaveSpawnData.Num();
	CurrentWave = 0;

	StartWave();
	EndSale();
}

void ASGCGameMode::GameOver()
{

}

void ASGCGameMode::KillEnemy()
{
	WaveLeftEnemies--;
}

// Аукцион на продажу патронов
void ASGCGameMode::StartSale()
{
	bIsSale = true;
	GetWorldTimerManager().ClearTimer(NextSaleTimerHandle);

	CurrentPriceOfBullets = FMath::RandBool() ? PriceOfBulletsMax : PriceOfBulletsMin;
	BulletsForSale = FMath::RandBool() ? BulletsForSaleMax : BulletsForSaleMin;

	OnStartBulletsSale.Broadcast(BulletsForSale);
	GetWorldTimerManager().SetTimer(SaleCountdownTimerHandle, this, &ASGCGameMode::SetCurrentPriceOfBullets, StepSecondsOfCountdown, true);
}

void ASGCGameMode::EndSale()
{
	bIsSale = false;
	GetWorldTimerManager().ClearTimer(SaleCountdownTimerHandle);
	OnFinishBulletsSale.Broadcast();

	int32 SecondsToNextSale = FMath::RandRange(SecondsToSaleMin, SecondsToSaleMax);
	GetWorldTimerManager().SetTimer(NextSaleTimerHandle, this, &ASGCGameMode::StartSale, SecondsToNextSale, false);
}

void ASGCGameMode::SetCurrentPriceOfBullets()
{
	CurrentPriceOfBullets -= StepPriceOfCountdown;
	if (CurrentPriceOfBullets == 0)
	{
		GetWorldTimerManager().ClearTimer(SaleCountdownTimerHandle);
	}
}

// Волны
void ASGCGameMode::StartWave()
{
	CurrentWaveSpawnData = WaveSpawnData[CurrentWave];
	for (auto EnemyData : CurrentWaveSpawnData.EnemiesData)
	{
		WaveLeftEnemies += EnemyData.EnemiesAmount;
	}
	SpawnWave();
}

void ASGCGameMode::SpawnWave()
{
	if (CurrentWaveSpawnData.EnemiesData.Num() == 0) return;

	int32 MaxSpawn = ((CurrentWaveSpawnData.AmountEnemiesSpawnAtOnce != 0) && (CurrentWaveSpawnData.SecondsBetweenSpawn != 0)) ? CurrentWaveSpawnData.AmountEnemiesSpawnAtOnce : WaveLeftEnemies;

	for (int32 EnemyNum = 0; EnemyNum < MaxSpawn; EnemyNum++)
	{
		int32 EnemyClassIndex = (CurrentWaveSpawnData.EnemiesData.Num() > 1) ? FMath::RandRange(0, CurrentWaveSpawnData.EnemiesData.Num() - 1) : 0;
		
		auto Enemy = GetEnemySpawnVolume()->SpawnEnemy(CurrentWaveSpawnData.EnemiesData[EnemyClassIndex].EnemyClass);
		if (Enemy)
		{
			Enemy->GetHealthComponent()->OnDeath.AddUObject(this, &ASGCGameMode::KillEnemy);
		}

		CurrentWaveSpawnData.EnemiesData[EnemyClassIndex].EnemiesAmount--;
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
	TArray<AActor*> EnemySpawnsArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASGCEnemySpawnVolume::StaticClass(), EnemySpawnsArray);
	int32 Index = FMath::RandRange(0, EnemySpawnsArray.Num() - 1);
	return Cast<ASGCEnemySpawnVolume>(EnemySpawnsArray[Index]);
}

void ASGCGameMode::WaveOver()
{
	EndSale();

	GetWorldTimerManager().ClearAllTimersForObject(this);
	//GetWorldTimerManager().ClearTimer(NextSaleTimerHandle);

	//return GetWorld()->GetTimeSeconds();
}

bool ASGCGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);

//	GetWorldTimerManager().PauseTimer(GameWaveTimerHandle);
	return PauseSet;
}

bool ASGCGameMode::ClearPause()
{
	const auto PauseCleared = Super::ClearPause();
	if (PauseCleared)
	{
//		GetWorldTimerManager().UnPauseTimer(GameWaveTimerHandle);
	}
	return PauseCleared;
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