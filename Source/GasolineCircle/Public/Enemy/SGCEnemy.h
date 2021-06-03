// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SGCEnemy.generated.h"

UCLASS()
class GASOLINECIRCLE_API ASGCEnemy : public APawn
{
	GENERATED_BODY()
	
public:	
	ASGCEnemy();
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class USGCHealthComponent* GetHealthComponent() const { return HealthComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class USGCHealthComponent* HealthComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UFloatingPawnMovement* MovementComponent;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float Damage = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float OuterDamageRadius = 150.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float InnerDamageRadius = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float TimeBetweenDamage = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	bool bShowDamegeSphere = true;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	class UParticleSystem* DeathParticles;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
	class USoundCue* SpawnSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
	class USoundCue* DeathSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coins Spawn")
	TSubclassOf<class ASGCCoin> CoinsClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coins Spawn")
	float TimeBetweenCoinsSpawn = 3.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coins Spawn", meta = (ClampMin = "0", ClampMax = "100.0"))
	float ChanceToCoinsSpawnPercent = 30.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coins Spawn")
	int32 CoinsSpawnMin = 5;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coins Spawn")
	int32 CoinsSpawnMax = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coins Animation")
	bool bEnemyJump = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coins Animation", meta = (EditCondition = "bEnemyJump"))
	float JumpHeight = 90.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coins Animation", meta = (EditCondition = "bEnemyJump"))
	float JumpVelocity = 180.0f;

	virtual void BeginPlay() override;

private:
	class ASGCMainCharacter* PlayerPawn = nullptr;

	FTimerHandle DamageTimerHandle;
	FTimerHandle CoinSpawnHandle;

	float StartZ = 0;
	float MaxJumpZ = 0;
	bool bIsMoveUp = true;

	void ApplyDamage();
	void OnDeath();
	void MoveToPlayer();
	void RotateToPlayer();
	void EnemyJump(float DeltaTime);
	void SpawnCoins();
};
