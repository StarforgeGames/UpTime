#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/Weapon.h"
#include "ProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class UPTIME_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	AProjectileWeapon();

	/**
	 * \brief Fires the weapon by spawning projectiles.
	 */
	void FireShot() override;

protected:
	/**
	 * \brief The projectile class to use.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile Weapon")
	TSubclassOf<class AProjectile> ProjectileClass;
};
