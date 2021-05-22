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

void ASGCWeapon::Reload()
{

}

void ASGCWeapon::MakeShot()
{
	if (!GetWorld()) return;

	//if (!GetWorld() || IsAmmoEmpty())
	//{
	//	StopFire();
	//	return;
	//}
	if (ProjectileClass)
	{
		FVector tempVector = WeaponMesh->GetSocketLocation(MuzzleSocketName);
		FRotator tempRotator = WeaponMesh->GetSocketRotation(MuzzleSocketName);

		ASGCProjectile* TempProjectile = GetWorld()->SpawnActor<ASGCProjectile>(ProjectileClass, tempVector, tempRotator);
		TempProjectile->SetOwner(GetOwner());
	}
}