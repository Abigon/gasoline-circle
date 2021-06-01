// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "Items/SGCItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

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

	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &ASGCItem::OnOverlapBegin);
}

void ASGCItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRotate)
	{
		FRotator Rotation = GetActorRotation();
		Rotation.Yaw += DeltaTime * RotationRate;
		SetActorRotation(Rotation);
	}
}

void ASGCItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
