// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SGCWeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASOLINECIRCLE_API USGCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USGCWeaponComponent();

	virtual void StartFire();
	void StopFire();
	void Reload();


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class ASGCWeapon> WeaponClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName WeaponSocketName = "WeaponSocket";

	UPROPERTY()
	ASGCWeapon* CurrentWeapon = nullptr;



	virtual void BeginPlay() override;

	void SpawnWeapon();
		
};
