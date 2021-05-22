// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGCWeapon.generated.h"

UCLASS()
class GASOLINECIRCLE_API ASGCWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ASGCWeapon();

	void StartFire();
	void StopFire();
	void Reload();


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class ASGCProjectile>ProjectileClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName MuzzleSocketName = "Muzzle";
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TimeBetweenShots = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 MaxBullets = 90;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 TotalBullets = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 BulletsInClip = 15;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 StartBullets = 60;



	virtual void BeginPlay() override;

	void MakeShot();

private:
	FTimerHandle ShotTimerHandle;

};
