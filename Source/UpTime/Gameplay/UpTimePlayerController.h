// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "UpTimePlayerController.generated.h"

class USoundCue;

UCLASS()
class AUpTimePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, Category="Input")
	bool bIsKeyboardInput = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player")
	USoundCue* PlayerDeathMusic;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Player")
	UAudioComponent* PlayerDeathMusicComponent;

public:
	AUpTimePlayerController();

	/**
	 * Tells whether the player uses keyboard input or gamepad.
	 * @return True if keyboard input is used.
	 */
	FORCEINLINE bool IsKeyboardInput() const { return bIsKeyboardInput; }

protected:
	virtual void OnPossess(APawn* MyPawn) override;

	UFUNCTION()
	void OnPlayerDeath();
};
