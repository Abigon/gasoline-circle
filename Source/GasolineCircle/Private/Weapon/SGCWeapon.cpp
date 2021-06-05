// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "Weapon/SGCWeapon.h"
#include "Weapon/SGCProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


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

	ResetAmmo();
}

void ASGCWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopFire();
	Super::EndPlay(EndPlayReason);
}


// Начинаем стрельбу. Делаемтся выстрел и запускается таймер для стрельбы очередями
void ASGCWeapon::StartFire()
{
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASGCWeapon::MakeShot, TimeBetweenShots, true);
	MakeShot();
}


// При прекращении стрельбы останавливаем таймер
void ASGCWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}


// Производим выстрел
// При пустом магазине выдаем соответствующий звук
// Если патроны в магазине есть, то спаунится пуля
void ASGCWeapon::MakeShot()
{
	if (!GetWorld())
	{
		StopFire();
		return;
	}

	if (IsClipEmpty())
	{
		StopFire();
		UGameplayStatics::SpawnSoundAttached(EmptySound, WeaponMesh, MuzzleSocketName);
		return;
	}

	if (ProjectileClass)
	{
		FVector tempVector = WeaponMesh->GetSocketLocation(MuzzleSocketName);
		FRotator tempRotator = WeaponMesh->GetSocketRotation(MuzzleSocketName);

		ASGCProjectile* TempProjectile = GetWorld()->SpawnActor<ASGCProjectile>(ProjectileClass, tempVector, tempRotator);
		if (TempProjectile)
		{
			TempProjectile->SetOwner(GetOwner());
		}
		
		UGameplayStatics::SpawnSoundAttached(ShotSound, WeaponMesh, MuzzleSocketName);

		BulletsInClip--;
	}
}


// Добавление патронов в инвентарь 
void ASGCWeapon::AddBullets(int32 NewBullets)
{
	TotalBullets = FMath::Min(TotalBullets + NewBullets, MaxBullets);
}


// Перезардка магазина с соответствующим звуком
void ASGCWeapon::Reload()
{
	if (!IsAmmoEmpty())
	{
		int32 NeedBullets = MaxBulletsInClip - BulletsInClip;
		NeedBullets = NeedBullets <= TotalBullets ? NeedBullets : TotalBullets;
		BulletsInClip += NeedBullets;
		TotalBullets -= NeedBullets;
		UGameplayStatics::SpawnSoundAttached(ReloadSound, WeaponMesh, MuzzleSocketName);
	}
}


// Восстановление кол-ва патронов до стартового 
// Одновременно зарядка первого магазина
void ASGCWeapon::ResetAmmo()
{
	TotalBullets = StartBullets;
	BulletsInClip = 0;
	int32 NeedBullets = MaxBulletsInClip - BulletsInClip;
	NeedBullets = NeedBullets <= TotalBullets ? NeedBullets : TotalBullets;
	BulletsInClip += NeedBullets;
	TotalBullets -= NeedBullets;
}

