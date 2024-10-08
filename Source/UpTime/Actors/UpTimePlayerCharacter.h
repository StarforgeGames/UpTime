// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Common/ArmedActor.h"
#include "Actors/Common/CombatCharacter.h"
#include "GameFramework/Character.h"
#include "Gameplay/UpTimePlayerController.h"
#include "UpTimePlayerCharacter.generated.h"

class UPowerSystemComponent;
class IInteractable;
class UCameraComponent;
class USpringArmComponent;

UCLASS(Blueprintable)
class AUpTimePlayerCharacter : public ACombatCharacter, public IArmedActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	UDecalComponent* CursorToWorld;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Power System", meta=(AllowPrivateAccess="true"))
	UPowerSystemComponent* PowerSystemComponent;

	/** The type of weapon the player starts with. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player", meta=(AllowPrivateAccess="true"))
	TSubclassOf<AWeapon> StartWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat", meta=(AllowPrivateAccess="true"))
	AWeapon* Weapon;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Interactable", meta=(AllowPrivateAccess="true"))
	TScriptInterface<IInteractable> FocussedInteractable;

public:
	AUpTimePlayerCharacter();

protected:
	virtual void BeginPlay() override;

	/**
	 * Setup pawn-specific controls
	 * @param PlayerInputComponent The input component.
	 */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void OnPowerDrained();

	/**
	 * Increases the current power by the given amount.
	 * @param ChargeValue The amount to add.
	 */
	UFUNCTION(BlueprintCallable, Category="Power System")
	void ChargePower(int ChargeValue);

	void FocusInteractable(IInteractable* Interactable);
	void UnfocusInteractable();

	/**
	 * Equips the given weapon on the armed actor.
	 * @param NewWeapon The weapon instance.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Weapons")
	void Equip(AWeapon* NewWeapon);

	/** Wraps the call start firing using the interface execute method for input handling. */
	void HandleStartFiring();
	/** Wraps the call stop firing using the interface execute method for input handling. */
	void HandleStopFiring();

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

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Power System")
	float GetCurrentPower();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Power System")
	int GetCurrentPowerCells();

	virtual bool Kill(AController* EventInstigator, AActor* DamageCauser) override;

	virtual float GetHitpoints() const override;
	virtual void SetHitpoints(float NewHitpoints) override;

	UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	UDecalComponent* GetCursorToWorld() const { return CursorToWorld; }

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);

	void LookUp(float Value);
	void LookRight(float Value);

	/**
	 * Turns the player character in the direction of the mouse cursor.
	 */
	void RotateToMousePosition(AUpTimePlayerController* PlayerController);

	/**
	 * Interacts with the currently focused interactable.
	 */
	void OnInteract();
};
