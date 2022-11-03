#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Items/Weapons/MuzzleConfiguration.h"

#include "ArmedActor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UArmedActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for armed actors that can wield weapons.
 */
class UPTIME_API IArmedActor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:	
	/**
	 * \brief Starts firing the weapon.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Weapons")
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

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Weapons")
	void OnFiringShot();
};
