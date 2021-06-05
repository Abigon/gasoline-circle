// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGCEnemySpawnVolume.generated.h"


/*
	Класс точки спауна мобов-врагов
	Может заспаунить кол-во мобов, установленное в параметре
	При выборе параметра, будет спаунить только босса. На карте обязательно должен присутствовать минимум 1 экземпляр класса для спауна босса

*/


UCLASS()
class GASOLINECIRCLE_API ASGCEnemySpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ASGCEnemySpawnVolume();

	// Может ли объект спаунить еще
	bool IsCanSpawn() const { return SpawningCount > 0; }

	// Может ли обхект спаунить босса
	bool IsCanSpawnBoss() const { return bOnlyBossSpawn; }

	// Сколько еще обхект может спаунить мобов
	int32 GetSpawningCount() const { return SpawningCount; }

	//Спаун моба
	class ASGCEnemy* SpawnEnemy(TSubclassOf<ASGCEnemy> EnemyClass);

	// Сброс кол-ва спауна мобов на значение по умолчанию
	void Reset() { SpawningCount = DefaultSpawningCount; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UBoxComponent* SpawningBox;

	// Кол-во возможных спаунов мобов по умолчанию
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 DefaultSpawningCount = 5;

	// Спаунит ли эта точка босса
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	bool bOnlyBossSpawn = false;

	// Оставшееся кол-во спаунов мобов
	int32 SpawningCount = 0;

	virtual void BeginPlay() override;

	FVector GetSpawnPoint();
};
