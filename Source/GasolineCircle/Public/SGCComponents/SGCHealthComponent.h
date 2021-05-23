// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SGCHealthComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASOLINECIRCLE_API USGCHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	FOnDeathSignature OnDeath;
	
	USGCHealthComponent();

	bool IsDead() const { return FMath::IsNearlyZero(Health); }
	float GetHealthPercent() const { return Health / MaxHealth; }


protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0", ClampMax = "500.0"))
	float MaxHealth = 100.f;

private:
	float Health;

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void SetHealth(float NewHealth);
	void ApplyDamage(float Damage);
};
