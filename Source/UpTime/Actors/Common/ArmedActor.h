#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/MuzzleConfiguration.h"
#include "UObject/Interface.h"
#include "ArmedActor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UArmedActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for armed actors that can wield weapons. Defines methods for handling the weapons.
 */
class UPTIME_API IArmedActor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Starts firing the weapon.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Weapons")
	void StartFiring();

	/**
	 * Stops firing the weapon.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Weapons")
	void StopFiring();

	/**
	 * Checks whether the weapon can be fired.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Weapons")
	bool CanFire() const;

	/**
	 * Checks whether any weapon is currently being fired.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Weapons")
	bool IsFiring() const;

	/**
	 * Gameplay hook that is triggered when a shot is fired.
	 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Weapons")
	void OnFiringShot();

	/**
	 * Returns them positions of the weapon muzzles based on the muzzle configuration.
	 * @param MuzzleConfig tells the function how many muzzles there are.
	 * @return Array of all the muzzle locations.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Weapons")
	TArray<FVector> GetWeaponMuzzleLocations(EMuzzleConfiguration MuzzleConfig);

	/**
	 * Get the weapon of this armed actor.
	 * @return Pointer to the weapon of the armed actor.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Weapons")
	class AWeapon* GetWeapon() const;
};
