// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SGCGameMode.h"
#include "Core/SGCPlayerController.h"
#include "Character/SGCMainCharacter.h"
#include "Enemy/SGCEnemy.h"
#include "Enemy/SGCEnemySpawnVolume.h"
#include "SGCComponents/SGCHealthComponent.h"
#include "UI/SGCHUD.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"


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

	auto PlayerPawn = Cast<ASGCMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (PlayerPawn)
	{
		PlayerPawn->GetHealthComponent()->OnDeath.AddUObject(this, &ASGCGameMode::KillPlayer);
	}

	StartWave();
}

void ASGCGameMode::GameOver()
{
	EndSale();
	GetWorldTimerManager().ClearAllTimersForObject(this);

	for (auto Pawn : TActorRange<APawn>(GetWorld()))
	{
		if (Pawn)
		{
			Pawn->TurnOff();
			Pawn->DisableInput(nullptr);
		}
	}

}

void ASGCGameMode::KillEnemy()
{
	WaveLeftEnemies--;
	if (WaveLeftEnemies == 0) WaveOver();
}

void ASGCGameMode::KillPlayer()
{
	GameOver();
}

// Волны
void ASGCGameMode::StartWave()
{
	CurrentWaveSpawnData = WaveSpawnData[CurrentWave];
	for (auto EnemyData : CurrentWaveSpawnData.EnemiesData)
	{
		WaveLeftEnemies += EnemyData.EnemiesAmount;
	}

	TArray<AActor*> EnemySpawnsArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASGCEnemySpawnVolume::StaticClass(), EnemySpawnsArray);
	for (auto EnemySpawn : EnemySpawnsArray)
	{
		auto SpawnVolume = Cast<ASGCEnemySpawnVolume>(EnemySpawn);
		if (SpawnVolume) SpawnVolume->Reset();
	}

	SpawnWave();
	EndSale();
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
	TArray<AActor*> EnemySpawnsActorArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASGCEnemySpawnVolume::StaticClass(), EnemySpawnsActorArray);

	TArray<ASGCEnemySpawnVolume*> EnemySpawnsArray;

	for (auto EnemySpawnActor : EnemySpawnsActorArray)
	{
		auto SpawnVolume = Cast<ASGCEnemySpawnVolume>(EnemySpawnActor);
		if (SpawnVolume && SpawnVolume->IsCanSpawn())
		{
			EnemySpawnsArray.Add(SpawnVolume);
		}
	}

	int32 Index = FMath::RandRange(0, EnemySpawnsArray.Num() - 1);
	return EnemySpawnsArray[Index];
}

void ASGCGameMode::WaveOver()
{
	UE_LOG(LogTemp, Warning, TEXT("WaveOver"));
	EndSale();
	GetWorldTimerManager().ClearAllTimersForObject(this);

	CurrentWave++;
	if (CurrentWave < TotalWaves)
	{
		auto PlayerController = Cast<ASGCPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		if (PlayerController && PlayerController->GetPawn())
		{
			PlayerController->GetPawn()->Reset();
			RestartPlayer(PlayerController);
		}
		StartWave();
	}
	else
	{
		GameOver();
	}
}

bool ASGCGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);

	//GetWorldTimerManager().PauseTimer(WaveSpawnTimerHandle);
	//GetWorldTimerManager().PauseTimer(SaleCountdownTimerHandle);
	//GetWorldTimerManager().PauseTimer(NextSaleTimerHandle);
	return PauseSet;
}

bool ASGCGameMode::ClearPause()
{
	const auto PauseCleared = Super::ClearPause();
	if (PauseCleared)
	{
		//GetWorldTimerManager().UnPauseTimer(WaveSpawnTimerHandle);
		//GetWorldTimerManager().UnPauseTimer(SaleCountdownTimerHandle);
		//GetWorldTimerManager().UnPauseTimer(NextSaleTimerHandle);
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
