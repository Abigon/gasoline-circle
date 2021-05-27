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
	
}

ASGCEnemy* ASGCEnemySpawnVolume::SpawnEnemy(TSubclassOf<class ASGCEnemy> EnemyClass)
{
	if (!CanSpawn() || !GetWorld()) return nullptr;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* Actor = GetWorld()->SpawnActor<AActor>(EnemyClass, GetActorLocation(), GetActorRotation(), SpawnParams);
	if (Actor) SpawningCount--;
	if (SpawningCount == 0) Destroy();
	return Cast<ASGCEnemy>(Actor);
}