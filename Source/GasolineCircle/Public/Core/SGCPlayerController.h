// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Core/SGCGameMode.h"
#include "GameFramework/PlayerController.h"
#include "SGCPlayerController.generated.h"


UCLASS()
class GASOLINECIRCLE_API ASGCPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASGCPlayerController();
	void SetPlayerControlAvaible(bool bIsAvaible);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void OnPauseGame();
	void OnGameStateChanged(ESGCGameState State);
};
