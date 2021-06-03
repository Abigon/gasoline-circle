// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SGCPlayerController.h"

ASGCPlayerController::ASGCPlayerController()
{
    //bShowMouseCursor = true;
    //DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ASGCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		const auto GameMode = Cast<ASGCGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnGameStateChanged.AddUObject(this, &ASGCPlayerController::OnGameStateChanged);
		}
	}
}

void ASGCPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (!InputComponent) return;

	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &ASGCPlayerController::OnPauseGame);
}

void ASGCPlayerController::OnPauseGame()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;
	GetWorld()->GetAuthGameMode()->SetPause(this);
}

void ASGCPlayerController::OnGameStateChanged(ESGCGameState State)
{
	if (State == ESGCGameState::EGS_InProgress)
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
	}
}

void ASGCPlayerController::SetPlayerControlAvaible(bool bIsAvaible)
{
	if (!GetPawn()) return;

	if (bIsAvaible)
	{
		GetPawn()->EnableInput(this);
	}
	else
	{
		GetPawn()->DisableInput(this);
	}
}
