// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "UpTimePlayerController.generated.h"

UCLASS()
class AUpTimePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AUpTimePlayerController();
	
	/**
	 * \brief Tells whether the player uses keyboard input or gamepad.
	 * \return True if keyboard input is used.
	 */
	FORCEINLINE bool IsKeyboardInput() const { return bIsKeyboardInput; }

	UFUNCTION()
	void OnPlayerDeath();
	
protected:
	void OnPossess(APawn* MyPawn) override;
	
	UPROPERTY(BlueprintReadWrite, Category="Input")
	bool bIsKeyboardInput = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player")
	class USoundCue* PlayerDeathMusic;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Player")
	class UAudioComponent* PlayerDeathMusicComponent;
};


