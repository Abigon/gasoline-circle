// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "Character/SGCMainCharacter.h"
#include "Enemy/SGCEnemy.h"
#include "SGCComponents/SGCHealthComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"


ASGCEnemy::ASGCEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(80.f);
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

	GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &ASGCEnemy::ApplyDamage, TimeBetweenDamage, true);

}

void ASGCEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveToPlayer();
}

void ASGCEnemy::ApplyDamage()
{
	auto PlayerPawn = Cast<ASGCMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	//return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());

	DrawDebugSphere(GetWorld(), GetActorLocation(), OuterDamageRadius, 12, FColor::Red, false, 0.5f);
	if (InnerDamageRadius)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), InnerDamageRadius, 12, FColor::Green, false, 0.5f);
	}

	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), OuterDamageRadius, UDamageType::StaticClass(),	{ GetOwner() }, this, GetController(), true);
}

void ASGCEnemy::OnDeath()
{
	GetWorldTimerManager().ClearTimer(DamageTimerHandle);
	if (DeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathParticles, GetActorLocation(), FRotator(0.f), true);
	}
	Destroy();
}

void ASGCEnemy::MoveToPlayer()
{
	auto PlayerPawn = Cast<ASGCMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

//	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), PlayerPawn->GetActorLocation());
}