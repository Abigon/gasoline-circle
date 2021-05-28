// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "Enemy/SGCEnemySpawnVolume.h"
#include "Enemy/SGCEnemy.h"


ASGCEnemySpawnVolume::ASGCEnemySpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ASGCEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	Reset();
}

ASGCEnemy* ASGCEnemySpawnVolume::SpawnEnemy(TSubclassOf<class ASGCEnemy> EnemyClass)
{
	if (!IsCanSpawn() || !GetWorld()) return nullptr;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* Actor = GetWorld()->SpawnActor<AActor>(EnemyClass, GetActorLocation(), GetActorRotation(), SpawnParams);
	if (Actor) SpawningCount--;
	return Cast<ASGCEnemy>(Actor);
}