// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "Weapon/SGCWeapon.h"
#include "Weapon/SGCProjectile.h"

ASGCWeapon::ASGCWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
}

void ASGCWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponMesh);

	TotalBullets = StartBullets;
	Reload();
}

void ASGCWeapon::StartFire()
{
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASGCWeapon::MakeShot, TimeBetweenShots, true);
	MakeShot();
}

void ASGCWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ASGCWeapon::MakeShot()
{
	if (!GetWorld() || IsClipEmpty())
	{
		StopFire();
		return;
	}

	if (ProjectileClass)
	{
		FVector tempVector = WeaponMesh->GetSocketLocation(MuzzleSocketName);
		FRotator tempRotator = WeaponMesh->GetSocketRotation(MuzzleSocketName);

		ASGCProjectile* TempProjectile = GetWorld()->SpawnActor<ASGCProjectile>(ProjectileClass, tempVector, tempRotator);
		TempProjectile->SetOwner(GetOwner());

		BulletsInClip--;
	}
}

void ASGCWeapon::AddBullets(int32 NewBullets)
{
	TotalBullets = FMath::Max(TotalBullets + NewBullets, MaxBullets);
}

void ASGCWeapon::Reload()
{

	if (!IsAmmoEmpty())
	{
		int32 NeedBullets = MaxBulletsInClip - BulletsInClip;
		NeedBullets = NeedBullets <= TotalBullets ? NeedBullets : TotalBullets;
		BulletsInClip += NeedBullets;
		TotalBullets -= NeedBullets;
	}
}

