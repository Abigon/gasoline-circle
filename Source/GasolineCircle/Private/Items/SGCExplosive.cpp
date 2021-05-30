// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "Items/SGCExplosive.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Character/SGCMainCharacter.h"


void ASGCExplosive::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		const auto Character = Cast<ASGCMainCharacter>(OtherActor);
		if (Character)
		{
			Character->TakeDamage(DamageAmount, FDamageEvent(), nullptr, this);
			UGameplayStatics::PlaySound2D(this, PickupSound);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BoomParticles, GetActorLocation(), FRotator(0.f), true);

			if (CameraShake)
			{
				const auto Controller = Cast<APlayerController>(Character->GetController());
				if (!Controller || !Controller->PlayerCameraManager) return;
				Controller->PlayerCameraManager->StartCameraShake(CameraShake);
			}

			Destroy();
		}
	}
}