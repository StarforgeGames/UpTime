#include "EnemyCharacter.h"

#include "UpTime.h"
#include "Actors/UpTimePlayerCharacter.h"
#include "Gameplay/AI/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Weapons/Weapon.h"
#include "Gameplay/Pickup.h"
#include "Sound/SoundCue.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
	: 	Hitpoints(100.f),
	AttackRange(1500.f)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (DefaultWeapon != nullptr)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		Weapon = GetWorld()->SpawnActor<AWeapon>(DefaultWeapon, SpawnInfo);
		Weapon->OnEquipped(this);
	}
}

float AEnemyCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	const float RemainingDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if(EventInstigator)
	{
		const auto AIController = Cast<AEnemyAIController>(GetController());
		const auto PlayerCharacter = Cast<AUpTimePlayerCharacter>(EventInstigator->GetPawn());
		
		if(AIController && PlayerCharacter)
		{
			AIController->SetEnemy(PlayerCharacter);
		}
	}

	return RemainingDamage;
}

bool AEnemyCharacter::Kill(AController* EventInstigator, AActor* DamageCauser)
{
	if (!Super::Kill(EventInstigator, DamageCauser))
	{
		return false;
	}

	if (Weapon)
	{
		Weapon->StopFiring();
	}

	GetCharacterMovement()->DisableMovement();

	SpawnPickup();

	return true;
}

void AEnemyCharacter::StartFiring_Implementation()
{
	if (!CanFire())
	{
		return;
	}
	
	if (Weapon)
	{
		Weapon->StartFiring();
	}
}


void AEnemyCharacter::StopFiring_Implementation()
{
	if (Weapon)
	{
		Weapon->StopFiring();
	}
}

bool AEnemyCharacter::CanFire_Implementation() const
{
	if (!IsAlive())
	{
		return false;
	}

	return Weapon && Weapon->CanFire();
}

bool AEnemyCharacter::IsFiring_Implementation() const
{
	if (!IsAlive())
	{
		return false;
	}
	
	return Weapon && Weapon->GetCurrentState() == EWeaponState::Firing;
}

TArray<FVector> AEnemyCharacter::GetWeaponMuzzleLocations_Implementation(EMuzzleConfiguration MuzzleConfig)
{
	USkeletalMeshComponent* EnemyMesh = GetMesh();
	TArray<FVector> MuzzleLocations;
	
	switch (MuzzleConfig)
	{
		case EMuzzleConfiguration::Single:
		{
			const FVector Center = EnemyMesh->GetSocketLocation("Muzzle_C");
			MuzzleLocations.Add(Center);
			break;
		}

		case EMuzzleConfiguration::Dual:
		{			
			const FVector Left = EnemyMesh->GetSocketLocation("Muzzle_L");
			MuzzleLocations.Add(Left);
			const FVector Right = EnemyMesh->GetSocketLocation("Muzzle_R");
			MuzzleLocations.Add(Right);
			break;
		}

		case EMuzzleConfiguration::Triple:
		{			
			const FVector Center = EnemyMesh->GetSocketLocation("Muzzle_C");
			MuzzleLocations.Add(Center);
			const FVector Left = EnemyMesh->GetSocketLocation("Muzzle_L");
			MuzzleLocations.Add(Left);
			const FVector Right = EnemyMesh->GetSocketLocation("Muzzle_R");
			MuzzleLocations.Add(Right);
			break;
		}
	}

	return MuzzleLocations;
}

AWeapon* AEnemyCharacter::GetWeapon_Implementation() const
{
	return Weapon;
}

float AEnemyCharacter::GetHitpoints() const
{
	return Hitpoints;
}

void AEnemyCharacter::SetHitpoints(float NewHitpoints)
{
	Hitpoints = NewHitpoints;
}

float AEnemyCharacter::GetAttackRange() const
{
	return AttackRange;
}