// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGCEnemySpawnVolume.generated.h"

UCLASS()
class GASOLINECIRCLE_API ASGCEnemySpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ASGCEnemySpawnVolume();

	bool CanSpawn() const { return SpawningCount > 0; }

	int32 GetSpawningCount() const { return SpawningCount; }

	class ASGCEnemy* SpawnEnemy(TSubclassOf<class ASGCEnemy> EnemyClass);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	class UBoxComponent* SpawningBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sale")
	int32 SpawningCount = 5;


	virtual void BeginPlay() override;


};
