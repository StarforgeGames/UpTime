#pragma once

#include "Projectile.h"

#include "PointProjectile.generated.h"

/**
* Projectiles that deal damage to a single point, like bullets.
*/
UCLASS()
class UPTIME_API APointProjectile : public AProjectile
{
	GENERATED_BODY()

protected:
	/** Damage applied to the impact point. */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float Damage;

public:
	APointProjectile();

	/**
	* Deals damage to the hit actor.
	* @param Impact Detailed information about the impact.
	*/
	virtual void DealDamage(const FHitResult& Impact) override;
};
