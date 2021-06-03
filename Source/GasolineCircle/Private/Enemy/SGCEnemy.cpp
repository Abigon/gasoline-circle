// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "Enemy/SGCEnemy.h"
#include "Character/SGCMainCharacter.h"
#include "Items/SGCCoin.h"
#include "SGCComponents/SGCHealthComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


ASGCEnemy::ASGCEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
//	CollisionComponent->InitSphereRadius(80.f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	SetRootComponent(CollisionComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Mesh->SetupAttachment(CollisionComponent);

	HealthComponent = CreateDefaultSubobject<USGCHealthComponent>(TEXT("HealthComponent"));
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->MaxSpeed = 300.f;
}

void ASGCEnemy::BeginPlay()
{
	Super::BeginPlay();

	check(CollisionComponent);
	check(Mesh);

	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	HealthComponent->OnDeath.AddUObject(this, &ASGCEnemy::OnDeath);

	PlayerPawn = Cast<ASGCMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &ASGCEnemy::ApplyDamage, TimeBetweenDamage, true);
	GetWorldTimerManager().SetTimer(CoinSpawnHandle, this, &ASGCEnemy::SpawnCoins, TimeBetweenCoinsSpawn, true);

	StartZ = GetActorLocation().Z;
	MaxJumpZ = StartZ + JumpHeight;
	bIsMoveUp = true;

	if (GetWorld())
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SpawnSound, GetActorLocation());
	}
}

void ASGCEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveToPlayer();
	RotateToPlayer();
	EnemyJump(DeltaTime);
}

void ASGCEnemy::ApplyDamage()
{
	auto DistanceToPlayer = FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());

	if ((DistanceToPlayer >= InnerDamageRadius) && (DistanceToPlayer <= OuterDamageRadius))
	{
		PlayerPawn->TakeDamage(Damage, FDamageEvent(), GetController(), this);
	}
	//UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), OuterDamageRadius, UDamageType::StaticClass(),	{ GetOwner() }, this, GetController(), true);


	if (bShowDamegeSphere)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), OuterDamageRadius, 12, FColor::Red, false, 0.5f);
		if (InnerDamageRadius > 0)
		{
			DrawDebugSphere(GetWorld(), GetActorLocation(), InnerDamageRadius, 12, FColor::Green, false, 0.5f);
		}
	}
}

void ASGCEnemy::OnDeath()
{
	GetWorldTimerManager().ClearTimer(DamageTimerHandle);
	if (DeathParticles)
	{
		if (GetWorld())
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathParticles, GetActorLocation(), FRotator(0.f), true);
		}
	}
	Destroy();
}

void ASGCEnemy::MoveToPlayer()
{
	if (!PlayerPawn) return;
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), PlayerPawn->GetActorLocation());
}

void ASGCEnemy::RotateToPlayer()
{
	if (!PlayerPawn) return;
	FVector LookAtTargetClean = FVector(PlayerPawn->GetActorLocation().X, PlayerPawn->GetActorLocation().Y, GetActorLocation().Z);
	FVector StartLocation = GetActorLocation();
	FRotator TurretRotation = FVector(LookAtTargetClean - StartLocation).Rotation();
	SetActorRotation(TurretRotation);
}

void ASGCEnemy::EnemyJump(float DeltaTime)
{
	if (!bEnemyJump) return;

	FVector Location = GetActorLocation();
	if (bIsMoveUp)
	{
		Location.Z += DeltaTime * JumpVelocity;
	}
	else
	{
		Location.Z -= DeltaTime * JumpVelocity;
	}

	if (Location.Z >= MaxJumpZ)
	{
		Location.Z = MaxJumpZ;
		bIsMoveUp = false;
	}
	if (Location.Z <= StartZ)
	{
		Location.Z = StartZ;
		bIsMoveUp = true;
	}
	SetActorLocation(Location);
}

void ASGCEnemy::SpawnCoins()
{
	if (!CoinsClass) return;

	bool IsCoinSpawn = FMath::RandRange(0.f, 100.f) <= ChanceToCoinsSpawnPercent;
	if (!IsCoinSpawn) return;

	int32 CoinsAmount = FMath::RandBool() ? CoinsSpawnMax : CoinsSpawnMin;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto Actor = Cast<ASGCCoin>(GetWorld()->SpawnActor<AActor>(CoinsClass, GetActorLocation(), FRotator(0), SpawnParams));
	if (Actor)
	{
		Actor->SetAmount(CoinsAmount);
	}
}