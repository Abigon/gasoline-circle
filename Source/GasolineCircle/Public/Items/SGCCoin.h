// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/SGCItem.h"
#include "SGCCoin.generated.h"

UCLASS()
class GASOLINECIRCLE_API ASGCCoin : public ASGCItem
{
	GENERATED_BODY()
	
public:	
	void SetAmount(int32 NewAmount) { Amount = NewAmount; }

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coin")
	int32 Amount = 1;

};
