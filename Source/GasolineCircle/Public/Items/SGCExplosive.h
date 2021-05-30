// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Items/SGCItem.h"
#include "SGCExplosive.generated.h"


UCLASS()
class GASOLINECIRCLE_API ASGCExplosive : public ASGCItem
{
	GENERATED_BODY()
	
public:
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosive")
	float DamageAmount = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive")
	class UParticleSystem* BoomParticles;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosive")
	TSubclassOf<class UCameraShakeBase>CameraShake;
};
