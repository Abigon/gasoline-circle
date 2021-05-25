// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SGCGameMode.h"
#include "Character/SGCMainCharacter.h"
#include "UI/SGCHUD.h"
#include "Core/SGCPlayerController.h"


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

	StartWave();
	EndSale();
}

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

void ASGCGameMode::GameOver()
{

}

void ASGCGameMode::WaveOver()
{
	EndSale();

	GetWorldTimerManager().ClearAllTimersForObject(this);
	//GetWorldTimerManager().ClearTimer(NextSaleTimerHandle);
	//GetWorldTimerManager().ClearTimer(GameWaveTimerHandle);
}

void ASGCGameMode::StartWave()
{
	//GetWorldTimerManager().SetTimer(GameWaveTimerHandle, nullptr, 1.f, true);
}

float ASGCGameMode::GetWaveTimerRate() const
{
	if (!GetWorld()) return 0;
	return GetWorld()->GetTimeSeconds(); //GetWorldTimerManager().GetTimerRate(GameWaveTimerHandle);
}

bool ASGCGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);

	GetWorldTimerManager().PauseTimer(GameWaveTimerHandle);
	return PauseSet;
}

bool ASGCGameMode::ClearPause()
{
	const auto PauseCleared = Super::ClearPause();
	if (PauseCleared)
	{
		GetWorldTimerManager().UnPauseTimer(GameWaveTimerHandle);
	}
	return PauseCleared;
}