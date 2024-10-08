#pragma once

#include "CoreMinimal.h"
#include "MuzzleConfiguration.h"
#include "WeaponState.h"
#include "GameFramework/Actor.h"

#include "Weapon.generated.h"

class USoundCue;
/**
 * Base class for all weapons.
 */
UCLASS()
class UPTIME_API AWeapon : public AActor
{
	GENERATED_BODY()

protected:
	/**
	 * Is fire sound looped?
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	bool bIsLoopedFireSound;

	/**
	 * Single fire sound (bLoopedFireSound not set).
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* FiringSound;

	/**
	 * Looped fire sound (bLoopedFireSound set).
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* FiringLoopSound;

	/**
	 * Finished burst sound (bLoopedFireSound set).
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* FireFinishSound;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* FiringEffect;

	/**
	 * Firing audio component (bLoopedFireSound set).
	 */
	UPROPERTY(Transient)
	UAudioComponent* FiringAudioComponent;

	/**
	 * Time between two consecutive shots.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	float TimeBetweenShots;

	/**
	 * Time between two consecutive shots.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	EMuzzleConfiguration MuzzleConfiguration;

	bool bWantsToFire;

	float LastTimeFiring;

	EWeaponState CurrentState;

	/**
	 * Handle for efficient management of HandleFiring timer.
	 */
	FTimerHandle TimerHandle_HandleFiring;

public:
	AWeapon();

	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;

public:
	/**
	 * Equips a weapon to the new owner.
	 * @param NewOwner The owner of the weapon.
	 */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void OnEquipped(TScriptInterface<class IArmedActor> NewOwner);

	/**
	 * Starts the firing of the weapon if it can be fired right now.
	 */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void StartFiring();

	/**
	 * Stops the firing of the weapon.
	 */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void StopFiring();

	/**
	 * Checks if weapon can be fired at the moment.
	 * @return True if the weapon is ready to fire.
	 */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool CanFire() const;

	/**
	 *  Returns the current weapon state.
	 * @return The current weapon state.
	 */
	EWeaponState GetCurrentState() const;

	/**
	 * Sets the current state of the weapon.
	 * @param NewState The new state to set.
	 */
	void SetCurrentState(const EWeaponState NewState);

protected:
	/**
	 * Handles the firing of the weapon.
	 */
	virtual void HandleFiring();

	/**
	 * Fires a single shot of the weapon.
	 */
	virtual void FireShot() PURE_VIRTUAL(AWeapon::FireShot,)

	/**
	 * Called to do the cosmetic fx for firing.
	 */
	virtual void SimulateWeaponFire();

	/**
	 * Plays the weapon sounds.
	 * @param Sound The sound to play.
	 * @return The audio component that is playing the sound.
	 */
	UAudioComponent* PlaySound(USoundCue* Sound) const;

	/**
	 * Called to stop cosmetic fx (e.g. for a looping shot).
	 */
	virtual void StopSimulatingWeaponFire();

	/**
	 * Returns the current aiming direction.
	 * @return Vector of the aim direction.
	 */
	FVector GetAimDirection() const;
};
