#include "RadialProjectile.h"

#include "Kismet/GameplayStatics.h"
#include "Items/Weapons/ProjectileWeapon.h"


ARadialProjectile::ARadialProjectile()
	: BaseDamage(20.f),
	DamageRadius(100.f)
{}

void ARadialProjectile::DealDamage(const FHitResult& HitResult)
{
	const APawn* WeaponOwner = Cast<APawn>(OwnerWeapon->GetOwner());
	if (!ensureMsgf(WeaponOwner, TEXT("Weapon owner not set or not a pawn, cannot deal radial damage")))
	{
		return;
	}

	const AActor* HitActor = HitResult.GetActor();
	if (!ensureMsgf(HitActor, TEXT("Could not retrieve hit Actor, cannot deal radial damage")))
	{
		return;
	}

	FVector HitFromDirection = HitActor->GetActorLocation() - WeaponOwner->GetActorLocation();
	HitFromDirection.Normalize();

	HitResult.Component->AddRadialImpulse(HitResult.ImpactPoint, DamageRadius, 25000.0f, RIF_Constant);

	UGameplayStatics::ApplyRadialDamage(
		this, 
		BaseDamage, 
		GetActorLocation(), 
		DamageRadius, 
		DamageType, 
		TArray<AActor*>(), 
		this, 
		WeaponOwner->GetController(),
		true);
}
