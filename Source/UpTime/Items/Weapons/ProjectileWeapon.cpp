#include "ProjectileWeapon.h"

#include "UpTime.h"
#include "Actors/UpTimePlayerCharacter.h"
#include "Projectiles/Projectile.h"

AProjectileWeapon::AProjectileWeapon()
{}

void AProjectileWeapon::FireShot()
{
	APawn* ShotInstigator = Cast<APawn>(GetOwner());
	const IArmedActor* WeaponOwner = Cast<IArmedActor>(ShotInstigator);

	if (!ensureMsgf(WeaponOwner, TEXT("Weapon must have an owner to be fired")))
	{
		UE_LOG(LogUpTime, Error, TEXT("Weapon has no owner to be fired!"));
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = ShotInstigator;

	const FVector ShootDir = GetAimDirection();
	TArray<FVector> MuzzleLocations =
		IArmedActor::Execute_GetWeaponMuzzleLocations(ShotInstigator, MuzzleConfiguration);

	for (const FVector Location : MuzzleLocations)
	{
		const auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileClass,
			Location,
			ShootDir.Rotation(),
			SpawnParams);

		if (!ensureMsgf(Projectile, TEXT("Could not spawn projectile")))
		{
			return;
		}

		float ForwardSpeed = 0;
		// Workaround: AI-controlled pawns have crazy high velocity which causes the projectiles to be visually displaced
		if (ShotInstigator->IsPlayerControlled())
		{
			ForwardSpeed = ShotInstigator->GetVelocity().Size();
		}

		Projectile->Launch(ForwardSpeed);
	}
}
