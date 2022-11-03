#pragma once

#include "GameFramework/Actor.h"

#include "Projectile.generated.h"

class USphereComponent;
class UParticleSystemComponent;
class AProjectileWeapon;

/**
* \brief Basic implementation of a weapons' projectile.
*/
UCLASS(Abstract, Blueprintable)
class UPTIME_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile(const FObjectInitializer& ObjectInitializer);

	void PostInitializeComponents() override;

	/**
	 * \brief Launches the projectile taking the speed of the firing actor in the weapon direction into consideration.
	 * \param OwnerSpeed Speed of the firing actor into the direction the weapon is facing.
	 */
	void Launch(float OwnerSpeed) const;

	/**
	 * \brief Called when the projectile hit something.
	 * \param HitResult The detailed result of the hit.
	 */
	 // ReSharper disable CppFunctionIsNotImplemented
	UFUNCTION(BlueprintNativeEvent, Category = "Combat")
	void OnImpact(const FHitResult& HitResult);

	bool OnImpact_Validate(const FHitResult& HitResult) { return true; };
	void OnImpact_Implementation(const FHitResult& HitResult);

	/**
	 * \brief Simulates the effect of the impact from the projectiles POV.
	 * \param HitResult The detailed result of the hit.
	 */
	 // ReSharper disable CppFunctionIsNotImplemented
	UFUNCTION(BlueprintNativeEvent, Category = "Combat")
		void OnSimulateImpact(const FHitResult& HitResult);

	bool OnSimulateImpact_Validate(const FHitResult& HitResult) { return true; };
	void OnSimulateImpact_Implementation(const FHitResult& HitResult);

	/**
	 * \brief Deals damage to the hit actor.
	 * \param Impact Detailed information about the impact.
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void DealDamage(const FHitResult& Impact) PURE_VIRTUAL(AProjectile::DealDamage, );

protected:
	/**
	 * \brief Shutdown projectile and prepare for destruction.
	 */
	void DisableAndDestroy();


	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	class UProjectileMovementComponent* MovementComponent;

	/** Life-time until the projectile gets automatically destroyed. */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float ProjectileLife;

	/** The type of damage to apply. */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleInstanceOnly, Category = "Projectile")
	class AProjectileWeapon* OwnerWeapon;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	class UParticleSystemComponent* ImpactEffectComponent;
};
