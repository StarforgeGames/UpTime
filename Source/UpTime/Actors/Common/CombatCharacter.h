#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatCharacter.generated.h"

class USoundCue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathDelegate);

UCLASS()
class UPTIME_API ACombatCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/** Gameplay hook that allows to react to the death of this character. */
	UPROPERTY(BlueprintAssignable)
	FOnDeathDelegate OnDeath;

protected:
	/** Indicates whether the combatant can be damaged or destroyed. */
	UPROPERTY(EditAnywhere, Category = "Combat")
	bool bIsInvulnerable;

	bool bIsDying;

	/** Sound when the enemy is hit. */
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundCue* HitSound;

	/** Effect when the enemy is hit. */
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* HitEffect;

	/** Sound when the enemy is killed. */
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundCue* DeathSound;

	/** Effect when the enemy is killed. */
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* DeathEffect;

public:
	ACombatCharacter();

	/**
	 * Called when the combatant takes damage.
	 * @param Damage The amount of damage taken by this character.
	 * @param DamageEvent The kind of damage event.
	 * @param EventInstigator The instigator of the damage event.
	 * @param DamageCauser The actor that actually caused the damage.
	 * @return The amount of damage taken by this character.
	 */
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
		AActor* DamageCauser) override;

	/**
	 * Simulates the effect when the combatant is being hit.
	 * @param DamageTaken The amount of damage taken by the hit.
	 * @param DamageType The kind of damage.
	 * @param EventInstigator The instigator that caused the event.
	 * @param DamageCauser The actual actor that caused the damage.
	 */
	// ReSharper disable CppFunctionIsNotImplemented
	UFUNCTION(BlueprintNativeEvent, Category = "Combat")
	void SimulateHit(float DamageTaken, TSubclassOf<UDamageType> DamageType, AController* EventInstigator,
		AActor* DamageCauser);

	/**
	 * Kill this combat character and clean up anything related to it before its destroyed.
	 * @param EventInstigator The instigator that caused the event.
	 * @param DamageCauser The actual actor that caused the damage.
	 * @return True if the combat character can be killed.
	 */
	bool virtual Kill(AController* EventInstigator, AActor* DamageCauser);

	/**
	 * Simulates the effect when the combatant dies.
	 * @param EventInstigator The instigator that caused the event.
	 * @param DamageCauser The actual actor that caused the damage.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Combat")
	void SimulateDeath(AController* EventInstigator, AActor* DamageCauser);

	/**
	 * Checks the current health of the combatant.
	 * @return The amount of hitpoints.
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual float GetHitpoints() const PURE_VIRTUAL(ACombatCharacter::GetHitpoints, return 0.f;)

	/**
	 * Sets the current health of the combatant.
	 * @param NewHitpoints The amount of hitpoints.
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void SetHitpoints(float NewHitpoints) PURE_VIRTUAL(ACombatCharacter::SetHitpoints)

	/**
	 * Indicates whether the character can be damaged or destroyed.
	 * @return True if the character cannot be damaged or destroyed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsInvulnerable() const;

	/**
	 * Sets whether the character can be damaged or destroyed.
	 * @param IsInvulnerable True if the character cannot be damaged or destroyed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetInvulernable(bool IsInvulnerable);

	/**
	 * Checks the state of the combatant to determine if it can die right now.
	 * @param EventInstigator The instigator that caused the event.
	 * @param DamageCauser The actual actor that caused the damage.
	 * @return True if the pawn can die in the current state.
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool CanDie(AController* EventInstigator, AActor* DamageCauser) const;

	UFUNCTION(BlueprintCallable, Category="Combat")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsDying() const;
};
