// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!


#include "Character/SGCMainCharacter.h"
#include "Core/SGCGameMode.h"
#include "SGCComponents/SGCWeaponComponent.h"
#include "SGCComponents/SGCHealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

ASGCMainCharacter::ASGCMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; 
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f); 

	// Create a camera boom (pulls in towards the player if there is a collision)
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = CameraZoomDefault;
	SpringArmComponent->SetRelativeRotation(FRotator(-80.f, 0.f, 0.f));
	SpringArmComponent->bDoCollisionTest = false;

	// Create a camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	HealthComponent = CreateDefaultSubobject<USGCHealthComponent>(TEXT("HealthComponent"));
	WeaponComponent = CreateDefaultSubobject<USGCWeaponComponent>(TEXT("WeaponComponent"));
}

void ASGCMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	HealthComponent->OnDeath.AddUObject(this, &ASGCMainCharacter::OnDeath);

	CoinAmount = 0;
}

void ASGCMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASGCMainCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASGCMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASGCMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASGCMainCharacter::TurnAtRate);

	PlayerInputComponent->BindAction("CameraZoomIn", IE_Pressed, this, &ASGCMainCharacter::CameraZoomIn);
	PlayerInputComponent->BindAction("CameraZoomOut", IE_Pressed, this, &ASGCMainCharacter::CameraZoomOut);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USGCWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USGCWeaponComponent::StopFire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &USGCWeaponComponent::Reload);
	PlayerInputComponent->BindAction("BuyBullets", IE_Pressed, this, &ASGCMainCharacter::TryBuyBullets);
}

void ASGCMainCharacter::CameraZoomIn()
{
	float CurrentTargetArmLength = SpringArmComponent->TargetArmLength;
	SpringArmComponent->TargetArmLength = FMath::Max(CurrentTargetArmLength - CameraZoomSpeed, CameraZoomMin);
}

void ASGCMainCharacter::CameraZoomOut()
{
	float CurrentTargetArmLength = SpringArmComponent->TargetArmLength;
	SpringArmComponent->TargetArmLength = FMath::Min(CurrentTargetArmLength + CameraZoomSpeed, CameraZoomMax);
}
void ASGCMainCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASGCMainCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASGCMainCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ASGCMainCharacter::OnDeath()
{
	//PlayAnimMontage(DeathAnimMontage);
	GetCharacterMovement()->DisableMovement();

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponComponent->StopFire();

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
}

void ASGCMainCharacter::AddCoins(int32 Coins)
{
	if (MaxCoinAmount != 0)
	{
		CoinAmount = FMath::Max(CoinAmount + Coins, MaxCoinAmount);
	}
	else
	{
		CoinAmount += Coins;
	}
}

bool ASGCMainCharacter::PayCoins(int32 Coins)
{
	if (CanPay(Coins))
	{
		CoinAmount -= Coins;
		return true;
	}
	return false;
}

bool ASGCMainCharacter::BuyBullets(int32 Coins, int32 Bullets)
{
	if (PayCoins(Coins))
	{
		WeaponComponent->AddCurrentBullets(Bullets);
		return true;
	}
	return false;
}

void ASGCMainCharacter::TryBuyBullets()
{
	if (!GetWorld()) return;
	auto GameMode = Cast<ASGCGameMode>(GetWorld()->GetAuthGameMode());

	if (!GameMode) return;
	if (!GameMode->IsSale()) return;

	if (BuyBullets(GameMode->GetCurrentPriceOfBullets(), GameMode->GetBulletsForSale()))
	{
		GameMode->EndSale();
	}
}