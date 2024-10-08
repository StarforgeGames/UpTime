#include "PointProjectile.h"

#include "Kismet/GameplayStatics.h"
#include "Items/Weapons/ProjectileWeapon.h"


APointProjectile::APointProjectile()
	: Damage(20.f)
{}

void APointProjectile::DealDamage(const FHitResult& HitResult)
{
	const APawn* WeaponOwner = Cast<APawn>(OwnerWeapon->GetOwner());
	if (!ensureMsgf(WeaponOwner, TEXT("Weapon owner not set or not a pawn, cannot deal point damage")))
	{
		return;
	}

	AActor* HitActor = HitResult.GetActor();
	if (!ensureMsgf(HitActor, TEXT("Could not retrieve hit Actor, cannot deal point damage")))
	{
		return;
	}

	FVector HitFromDirection = HitActor->GetActorLocation() - WeaponOwner->GetActorLocation();
	HitFromDirection.Normalize();

	UGameplayStatics::ApplyPointDamage(
		HitActor,
		Damage,
		HitFromDirection,
		HitResult,
		WeaponOwner->GetController(),
		this,
		DamageType);
}
