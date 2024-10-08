#pragma once

#include "Projectile.h"

#include "RadialProjectile.generated.h"

/**
* Projectiles that deal damage to an area of effect, like exploding rockets.
*/
UCLASS()
class UPTIME_API ARadialProjectile : public AProjectile
{
	GENERATED_BODY()

protected:
	/**
	* Damage applied to the area of effect.
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float BaseDamage;

	/**
	* The area affected by the damage.
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float DamageRadius;

public:
	ARadialProjectile();

	/**
	* Deals damage to the hit actor.
	* @param Impact Detailed information about the impact.
	*/
	virtual void DealDamage(const FHitResult& Impact) override;
};
