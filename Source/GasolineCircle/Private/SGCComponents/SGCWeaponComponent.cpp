// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "SGCComponents/SGCWeaponComponent.h"
#include "Weapon/SGCWeapon.h"
#include "GameFramework/Character.h"

USGCWeaponComponent::USGCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USGCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(WeaponClass, TEXT("Our Character must have a WeaponClass"));

	SpawnWeapon();
}

// При окончании игры останавливаем стрельбу и удаляем оружие
// Иначе в случае сброса чарактера, оружие останется на карте
void USGCWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopFire();
	CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentWeapon->Destroy();
	CurrentWeapon = nullptr;

	Super::EndPlay(EndPlayReason);
}


// Производим спаун оружия и присоединяем его к персонажу
void USGCWeaponComponent::SpawnWeapon()
{
	auto Character = Cast<ACharacter>(GetOwner());
	if (!Character || !GetWorld()) return;

	auto Weapon = GetWorld()->SpawnActor<ASGCWeapon>(WeaponClass);
	if (Weapon)
	{
		Weapon->SetOwner(Character);
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
		Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponSocketName);
		CurrentWeapon = Weapon;
	}
}

// Начинаем стрельбу. Вызывается при нажатии соотвествующей кнопки управления
void USGCWeaponComponent::StartFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StartFire();
}

// Прекращаем стрельбу. Вызывается при отпустанни соотвествующей кнопки управления
void USGCWeaponComponent::StopFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
}

// Перезарядка оружия. Вызывается при нажатии соотвествующей кнопки управления
void USGCWeaponComponent::Reload()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
	CurrentWeapon->Reload();
}

// Сбрасывает кол-во патронов в текущем оружии на первоначальные значения
void USGCWeaponComponent::ResetDefaultAmmo()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
	CurrentWeapon->ResetAmmo();
}

// Геттер для вижджета. Возвращается кол-во патронов текущего оружия в инвентаре
int32 USGCWeaponComponent::GetCurrentTotalBullets() const
{
	if (CurrentWeapon) return CurrentWeapon->GetTotalBullets();
	return 0;
}

// Геттер для вижджета. Возвращает кол-во патронов текущего оружия в обойме
int32 USGCWeaponComponent::GetCurrentBulletsInClip() const
{
	if (CurrentWeapon) return CurrentWeapon->GetBulletsInClip();
	return 0;
}

// Добавляет патроны в инвентарь для текущего оружия
void USGCWeaponComponent::AddCurrentBullets(int32 Bullets)
{ 
	if (!CurrentWeapon) return;
	CurrentWeapon->AddBullets(Bullets); 
}