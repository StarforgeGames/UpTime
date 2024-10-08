#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/Weapon.h"
#include "ProjectileWeapon.generated.h"

class AProjectile;

/**
 * 
 */
UCLASS()
class UPTIME_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()

protected:
	/**
	 * The projectile class to use.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile Weapon")
	TSubclassOf<AProjectile> ProjectileClass;

public:
	AProjectileWeapon();

	/**
	 * Fires the weapon by spawning projectiles.
	 */
	virtual void FireShot() override;
};
