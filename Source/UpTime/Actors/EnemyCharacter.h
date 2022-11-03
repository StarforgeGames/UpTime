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

public:
	AEnemyCharacter();

	void PostInitializeComponents() override;

	float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
		AActor* DamageCauser) override;
		
	bool Kill(AController* EventInstigator, AActor* DamageCauser) override;
	
	/**
	 * \brief Starts firing the weapon.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Weapons")
	// ReSharper disable CppHidingFunction
	void StartFiring();
	
	/**
	 * \brief Stops firing the weapon.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Weapons")
	void StopFiring();

	/**
	* \brief Checks whether the weapon can be fired.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Weapons")
	bool CanFire() const;

	/**
	* \brief Checks whether any weapon is currently being fired.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Weapons")
	bool IsFiring() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Weapons")
	TArray<FVector> GetWeaponMuzzleLocations(EMuzzleConfiguration MuzzleConfig);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Weapons")
	class AWeapon* GetWeapon() const;	
	// ReSharper restore CppHidingFunction
	
	float GetHitpoints() const override;
	
	void SetHitpoints(float NewHitpoints) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category="Combat")
	void SpawnPickup();
		
	UFUNCTION(BlueprintGetter, Category = "Combat")
	float GetAttackRange() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TSubclassOf<class AWeapon> DefaultWeapon;

	UPROPERTY(EditInstanceOnly, Category = "Weapons")
	class AWeapon* Weapon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<class APickup> PickupToSpawn;

	/** The current hitpoints of the enemy. */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float Hitpoints;

	/** The maximum attack range of the enemy. */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetAttackRange, Category = "Combat")
	float AttackRange;
};
