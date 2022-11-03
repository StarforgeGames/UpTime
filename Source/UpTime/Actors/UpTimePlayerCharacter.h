// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/UpTimePlayerController.h"
#include "Actors/Common/ArmedActor.h"
#include "Actors/Common/CombatCharacter.h"
#include "GameFramework/Character.h"

#include "UpTimePlayerCharacter.generated.h"

UCLASS(Blueprintable)
class AUpTimePlayerCharacter : public ACombatCharacter, public IArmedActor
{
	GENERATED_BODY()

public:
	AUpTimePlayerCharacter();

protected:
	void BeginPlay() override;
	
	/**
	 * \brief Setup pawn-specific controls
	 * \param PlayerInputComponent The input component.
	 */
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:	
	void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void OnPowerDrained();
	
	/**
	 * \brief Increases the current power by the given amount.
	 * \param ChargeValue The amount to add.
	 */
	UFUNCTION(BlueprintCallable, Category="Power System")
	void ChargePower(int ChargeValue);

	void FocusInteractable(class IInteractable* Interactable);
	void UnfocusInteractable();

	/**
	 * \brief Equips the given weapon on the armed actor.
	 * \param NewWeapon The weapon instance.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Weapons")
	void Equip(class AWeapon* NewWeapon);
	
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
	FORCEINLINE AWeapon* GetWeapon() const;
	// ReSharper restore CppHidingFunction

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Power System")
	float GetCurrentPower();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Power System")
	int GetCurrentPowerCells();

	bool Kill(AController* EventInstigator,	AActor* DamageCauser) override;

	float GetHitpoints() const override;
	void SetHitpoints(float NewHitpoints) override;
	
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UDecalComponent* GetCursorToWorld() const	{ return CursorToWorld; }

protected:
	
	void MoveForward(float Value);
	void MoveRight(float Value);

	void LookUp(float Value);
	void LookRight(float Value);
		
	/**
	 * \brief Turns the player character in the direction of the mouse cursor.
	 */
	void RotateToMousePosition(AUpTimePlayerController* PlayerController);

	/**
	 * \brief Interacts with the currently focused interactable.
	 */
	void OnInteract();	

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	class UDecalComponent* CursorToWorld;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Power System", meta=(AllowPrivateAccess="true"))
	class UPowerSystemComponent* PowerSystemComponent;
	
	/** The type of weapon the player starts with. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player", meta=(AllowPrivateAccess="true"))
	TSubclassOf<class AWeapon> StartWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat", meta=(AllowPrivateAccess="true"))
	class AWeapon* Weapon;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Interactable", meta=(AllowPrivateAccess="true"))
	class TScriptInterface<class IInteractable> FocussedInteractable;
};

