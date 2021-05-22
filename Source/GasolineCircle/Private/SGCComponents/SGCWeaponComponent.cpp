// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "Weapon/SGCWeapon.h"
#include "GameFramework/Character.h"
#include "SGCComponents/SGCWeaponComponent.h"

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

void USGCWeaponComponent::SpawnWeapon()
{
	auto Character = Cast<ACharacter>(GetOwner());
	if (!Character || !GetWorld()) return;

	auto Weapon = GetWorld()->SpawnActor<ASGCWeapon>(WeaponClass);
	if (Weapon)
	{
		//Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnEmptyClip);
		Weapon->SetOwner(Character);
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
		Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponSocketName);
		CurrentWeapon = Weapon;
	}
}

void USGCWeaponComponent::StartFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StartFire();
}

void USGCWeaponComponent::StopFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
}

void USGCWeaponComponent::Reload()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
	CurrentWeapon->Reload();
}