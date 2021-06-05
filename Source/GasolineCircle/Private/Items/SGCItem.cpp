// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "Items/SGCItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ASGCItem::ASGCItem()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	CollisionVolume->InitSphereRadius(50.f);
	SetRootComponent(CollisionVolume);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(CollisionVolume);
}

void ASGCItem::BeginPlay()
{
	Super::BeginPlay();

	check(CollisionVolume);
	check(Mesh);

	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &ASGCItem::OnCollisionVolumeOverlapBegin);
}

void ASGCItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Вращаем итем, если включена галка
	if (bRotate)
	{
		FRotator Rotation = GetActorRotation();
		Rotation.Yaw += DeltaTime * RotationRate;
		SetActorRotation(Rotation);
	}
}

void ASGCItem::OnCollisionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Необходимо перекрытие в наследниках, если нужна обработка
}

// Проигрываем звуки эффект перекрытия
void ASGCItem::PlayEffects()
{
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), PickupSound);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PickupParticles, GetActorLocation(), FRotator(0.f), true);
	}
}