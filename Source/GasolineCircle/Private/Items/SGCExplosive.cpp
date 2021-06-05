// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "Items/SGCExplosive.h"
#include "Character/SGCMainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


void ASGCExplosive::BeginPlay()
{
	Super::BeginPlay();
	Respawn();
}

void ASGCExplosive::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (GetWorldTimerManager().IsTimerActive(RespawnTimerHandle)) return;

	if (!OtherActor) return;

	const auto Character = Cast<ASGCMainCharacter>(OtherActor);
	if (!Character) return;

	Character->TakeDamage(DamageAmount, FDamageEvent(SGCDamageType), nullptr, this);

	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), PickupSound);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BoomParticles, GetActorLocation(), FRotator(0.f), true);
	}

	if (GetRootComponent())
	{
		GetRootComponent()->SetVisibility(false, true);
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASGCExplosive::Respawn, SecondsToRespawn, false);
	}
}

void ASGCExplosive::Respawn()
{
	GetWorldTimerManager().ClearTimer(RespawnTimerHandle);
	if (GetRootComponent())
	{
		GetRootComponent()->SetVisibility(true, true);
	}
}