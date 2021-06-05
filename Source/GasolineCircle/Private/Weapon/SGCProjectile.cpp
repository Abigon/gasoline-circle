// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "Weapon/SGCProjectile.h"
#include "Enemy/SGCEnemy.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ASGCProjectile::ASGCProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(12.f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	SetRootComponent(CollisionComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Mesh->SetupAttachment(CollisionComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	// Устанавливаем скорость пули
	MovementComponent->InitialSpeed = 1500.f;

	// Отключаем гравитацию, чтобы пуля летела гоизонтально
	MovementComponent->ProjectileGravityScale = 0.f;
}

void ASGCProjectile::BeginPlay()
{
	Super::BeginPlay();

	check(MovementComponent);
	check(CollisionComponent);
	check(Mesh);

	// Игнорируем стрелка, тчобы не было попадания стреляющего по себе
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);			

	CollisionComponent->OnComponentHit.AddDynamic(this, &ASGCProjectile::OnProjectileHit);
	SetLifeSpan(LifeSeconds);
}

void ASGCProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	MovementComponent->StopMovementImmediately();
	Destroy();

	// Наносим урон только врагам, остальных игнорируем
	const auto DamagedActor = Cast<ASGCEnemy>(OtherActor);
	if (!DamagedActor) return;

	const auto Pawn = Cast<APawn>(GetOwner());
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;
	DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), Controller, this);
}
