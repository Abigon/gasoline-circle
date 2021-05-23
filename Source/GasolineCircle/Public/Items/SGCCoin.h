// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGCCoin.generated.h"

UCLASS()
class GASOLINECIRCLE_API ASGCCoin : public AActor
{
	GENERATED_BODY()
	
public:	
	ASGCCoin();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	class USphereComponent* CollisionVolume;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coin | Sounds")
	class USoundCue* PickupSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coin | Rotation")
	bool bRotate = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coin | Rotation", meta = (EditCondition = "bRotate"))
	float RotationRate = 45.f;;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coin")
	int32 Amount = 1;


	virtual void BeginPlay() override;


private:	

};
