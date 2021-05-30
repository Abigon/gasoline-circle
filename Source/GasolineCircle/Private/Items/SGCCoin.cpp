// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "Character/SGCMainCharacter.h"
#include "Items/SGCCoin.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void ASGCCoin::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if (OtherActor)
	{
		const auto Character = Cast<ASGCMainCharacter>(OtherActor);
		if (Character)
		{
			Character->AddCoins(Amount);
			UGameplayStatics::PlaySound2D(this, PickupSound);
			Destroy();
		}
	}
}
