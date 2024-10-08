// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/ArmedActor.h"
#include "Common/CombatCharacter.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyDeathDelegate);

UCLASS()
class UPTIME_API AEnemyCharacter : public ACombatCharacter, public IArmedActor
{
	GENERATED_BODY()

protected:
	/** The type of weapon to spawn with the enemy by default. */
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TSubclassOf<AWeapon> DefaultWeapon;

	/** The weapon the enemy is wielding. */
	UPROPERTY(EditInstanceOnly, Category = "Weapons")
	AWeapon* Weapon;

	/** The pickup to spawn on enemy death. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<class APickup> PickupToSpawn;

	/** The current hitpoints of the enemy. */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float Hitpoints;

	/** The maximum attack range of the enemy. */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetAttackRange, Category = "Combat")
	float AttackRange;

public:
	AEnemyCharacter();

	virtual void PostInitializeComponents() override;

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
		AActor* DamageCauser) override;

	virtual bool Kill(AController* EventInstigator, AActor* DamageCauser) override;

	/**
	 * Starts firing the weapon.
	 */
	virtual void StartFiring_Implementation() override;

	/**
	 * Stops firing the weapon.
	 */
	virtual void StopFiring_Implementation() override;

	/**
	 * Checks whether the weapon can be fired.
	 */
	virtual bool CanFire_Implementation() const override;

	/**
	 * Checks whether any weapon is currently being fired.
	 */
	virtual bool IsFiring_Implementation() const override;

	virtual TArray<FVector> GetWeaponMuzzleLocations_Implementation(EMuzzleConfiguration MuzzleConfig) override;

	virtual AWeapon* GetWeapon_Implementation() const override;

	UFUNCTION(BlueprintImplementableEvent, Category="Combat")
	void SpawnPickup();

	virtual float GetHitpoints() const override;

	virtual void SetHitpoints(float NewHitpoints) override;

	UFUNCTION(BlueprintGetter, Category = "Combat")
	float GetAttackRange() const;
};
