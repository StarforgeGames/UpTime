#pragma once

#include "Projectile.h"

#include "PointProjectile.generated.h"

/**
* \brief Projectiles that deal damage to a single point, like bullets.
*/
UCLASS()
class UPTIME_API APointProjectile : public AProjectile
{
	GENERATED_BODY()
	
public:
	APointProjectile(const FObjectInitializer& ObjectInitializer);

	/**
	* \brief Deals damage to the hit actor.
	* \param Impact Detailed information about the impact.
	*/
	void DealDamage(const FHitResult& Impact) override;
	

protected:
	/** Damage applied to the impact point. */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float Damage;
};
