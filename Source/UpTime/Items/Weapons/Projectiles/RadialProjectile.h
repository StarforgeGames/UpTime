#pragma once

#include "Projectile.h"

#include "RadialProjectile.generated.h"

/**
* \brief Projectiles that deal damage to an area of effect, like exploding rockets.
*/
UCLASS()
class UPTIME_API ARadialProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	ARadialProjectile(const FObjectInitializer& ObjectInitializer);

	/**
	* \brief Deals damage to the hit actor.
	* \param Impact Detailed information about the impact.
	*/
	void DealDamage(const FHitResult& Impact) override;


protected:
	/**
	* \brief Damage applied to the area of effect.
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float BaseDamage;

	/**
	* \brief The area affected by the damage.
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float DamageRadius;
};
