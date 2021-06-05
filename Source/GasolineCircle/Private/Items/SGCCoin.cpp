// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "Items/SGCCoin.h"
#include "Character/SGCMainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void ASGCCoin::OnCollisionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnCollisionVolumeOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (!OtherActor) return;

	// Добавляем монеты только герою. Остальные взять их не могут
	const auto Character = Cast<ASGCMainCharacter>(OtherActor);
	if (Character)
	{
		Character->AddCoins(Amount);
		PlayEffects();
		Destroy();
	}
}
