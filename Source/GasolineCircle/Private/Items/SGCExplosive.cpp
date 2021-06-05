// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "Items/SGCExplosive.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


void ASGCExplosive::BeginPlay()
{
	Super::BeginPlay();
	Respawn();
}

void ASGCExplosive::OnCollisionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnCollisionVolumeOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// При запущенном таймере восстановления перекрытие игнорируется
	if (GetWorldTimerManager().IsTimerActive(RespawnTimerHandle)) return;

	if (!OtherActor) return;

	// Если объекта перекрывшего мину нет в списке он игнорируется
	if (!DamagedPawnsClasses.Contains(OtherActor->GetClass())) return;

	OtherActor->TakeDamage(DamageAmount, FDamageEvent(SGCDamageType), nullptr, this);

	PlayEffects();

	// Скрываем мину после взрыва и запускаем таймер появления
	if (GetRootComponent())
	{
		GetRootComponent()->SetVisibility(false, true);
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASGCExplosive::Respawn, SecondsToRespawn, false);
	}
}


// Восстановление мины на карте
void ASGCExplosive::Respawn()
{
	if (GetRootComponent())
	{
		GetRootComponent()->SetVisibility(true, true);
	}
}