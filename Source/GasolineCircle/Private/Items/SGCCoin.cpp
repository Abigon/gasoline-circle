// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "Items/SGCCoin.h"
#include "Character/SGCMainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void ASGCCoin::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor)
	{
		const auto Character = Cast<ASGCMainCharacter>(OtherActor);
		if (Character)
		{
			Character->AddCoins(Amount);
			if (GetWorld())
			{
				UGameplayStatics::PlaySound2D(GetWorld(), PickupSound);
			}
			Destroy();
		}
	}
}
