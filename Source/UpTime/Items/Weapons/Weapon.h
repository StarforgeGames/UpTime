#pragma once

#include "CoreMinimal.h"
#include "MuzzleConfiguration.h"
#include "WeaponState.h"
#include "GameFramework/Actor.h"

#include "Weapon.generated.h"

class USoundCue;
/**
 * \brief Base class for all weapons.
 */
UCLASS()
class UPTIME_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

	void Destroyed() override;

protected:
	void BeginPlay() override;

public:
	/**
	 * \brief Equips a weapon to the new owner.
	 * \param NewOwner The owner of the weapon.
	 */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void OnEquipped(TScriptInterface<class IArmedActor> NewOwner);

	/**
	 * \brief Starts the firing of the weapon if it can be fired right now.
	 */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void StartFiring();

	/**
	 * \brief Stops the firing of the weapon.
	 */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void StopFiring();

	/**
	 * \brief Checks if weapon can be fired at the moment.
	 * \return True if the weapon is ready to fire.
	 */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool CanFire() const;
	
	/**
	 * \brief  Returns the current weapon state.
	 * \return The current weapon state.
	 */
	EWeaponState GetCurrentState() const;

	/**
	 * \brief Sets the current state of the weapon.
	 * \param NewState The new state to set.
	 */
	void SetCurrentState(const EWeaponState NewState);

protected:
	/**
	 * \brief Handles the firing of the weapon.
	 */
	virtual void HandleFiring();

	/**
	 * \brief Fires a single shot of the weapon.
	 */
	virtual void FireShot() PURE_VIRTUAL(AWeapon::FireShot, )

	/**
	 * \brief Called to do the cosmetic fx for firing.
	 */
	virtual void SimulateWeaponFire();
		
	/**
	 * \brief Plays the weapon sounds.
	 * \param Sound The sound to play.
	 * \return The audio component that is playing the sound.
	 */
	class UAudioComponent* PlaySound(class USoundCue* Sound) const;

	/**
	 * \brief Called to stop cosmetic fx (e.g. for a looping shot).
	 */
	virtual void StopSimulatingWeaponFire();

	/**
	 * \brief Returns the current aiming direction.
	 * \return Vector of the aim direction.
	 */
	FVector GetAimDirection() const;

	/**
	 * \brief Is fire sound looped?
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	bool bIsLoopedFireSound;

	/**
	 * \brief Single fire sound (bLoopedFireSound not set).
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundCue* FiringSound;

	/**
	 * \brief Looped fire sound (bLoopedFireSound set).
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundCue* FiringLoopSound;

	/**
	 * \brief Finished burst sound (bLoopedFireSound set).
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundCue* FireFinishSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class UParticleSystem* FiringEffect;
	
	/**
	 * \brief Firing audio component (bLoopedFireSound set).
	 */
	UPROPERTY(Transient)
	class UAudioComponent* FiringAudioComponent;
		
	/**
	 * \brief Time between two consecutive shots.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	float TimeBetweenShots;
		
	/**
	 * \brief Time between two consecutive shots.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	EMuzzleConfiguration MuzzleConfiguration;
	
	bool bWantsToFire;
	
	float LastTimeFiring;
	
	EWeaponState CurrentState;

	/**
	 * \brief Handle for efficient management of HandleFiring timer.
	 */
	FTimerHandle TimerHandle_HandleFiring;
};
