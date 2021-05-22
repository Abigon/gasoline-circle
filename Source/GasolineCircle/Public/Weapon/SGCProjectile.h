// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGCProjectile.generated.h"

UCLASS()
class GASOLINECIRCLE_API ASGCProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ASGCProjectile();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* MovementComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float DamageAmount = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float LifeSeconds = 5.f;


	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
