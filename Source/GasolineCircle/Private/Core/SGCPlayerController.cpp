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

	// Подписываемся на события смены состояния игры
	if (GetWorld())
	{
		const auto GameMode = Cast<ASGCGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnGameStateChanged.AddUObject(this, &ASGCPlayerController::OnGameStateChanged);
		}
	}
}


// Назначаем обработчик поставноки на паузу
void ASGCPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (!InputComponent) return;

	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &ASGCPlayerController::OnPauseGame);
}

// Устанавливаем игру на паузу в GameMode
void ASGCPlayerController::OnPauseGame()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;
	GetWorld()->GetAuthGameMode()->SetPause(this);
}

// Изменяем видимость курсора и InputMode для состояния игры и всех других состояний
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

// Блокирует/разблокирует управление.
// Используется при обратном отсчете новой волны
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
