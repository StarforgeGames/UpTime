#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LevelGeneration/TileSet.h"

#include "UpTimeGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UPTIME_API UUpTimeGameInstance : public UGameInstance
{
	GENERATED_BODY()

	/** The current overall level the player has advanced to. */
	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetCurrentFloor, Category="Level")
	int CurrentFloor;

	/** The current level in the tile set the player has advanced to. */
	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetCurrentTileSetLevel, Category="Level")
	int CurrentTileSetLevel;

	/** The current level in the tile set the player has advanced to. */
	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetCurrentTileSetIndex, Category="Level")
	int CurrentTileSetIndex;

	/** The current level the player has advanced to. */
	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetCurrentTileSet, BlueprintSetter=SetCurrentTileSet, Category="Level")
	FTileSet CurrentTileSet;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=IsFirstLevel, Category="Level")
	bool bIsFirstLevel;

	/** The amount of power remaining at the end of a level to the player. */
	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetPlayerPower, Category="Player")
	float PlayerPower;

	/** The type of weapon the player carried at the end of a level. */
	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetPlayerWeapon, Category="Player")
	TSubclassOf<class AWeapon> PlayerWeapon;

public:
	UUpTimeGameInstance();

	UFUNCTION(BlueprintCallable, BlueprintGetter, Category="Level")
	int GetCurrentFloor() const;

	UFUNCTION(BlueprintCallable, BlueprintGetter, Category="Level")
	int GetCurrentTileSetLevel() const;

	UFUNCTION(BlueprintCallable, BlueprintGetter, Category="Level")
	int GetCurrentTileSetIndex() const;

	UFUNCTION(BlueprintCallable, Category="Level")
	void NextLevel(float CurrentPlayerPower, TSubclassOf<class AWeapon> CurrentWeapon);

	UFUNCTION(BlueprintCallable, BlueprintGetter, Category="Level")
	FTileSet GetCurrentTileSet() const;

	UFUNCTION(BlueprintCallable, BlueprintSetter, Category="Level")
	void SetCurrentTileSet(FTileSet NewTileSet);

	UFUNCTION(BlueprintCallable, BlueprintSetter, Category="Level")
	void StartNewTileSet(FTileSet NewTileSet);

	UFUNCTION(BlueprintCallable, Category="Level")
	bool IsCurrentTileSetFinished() const;

	UFUNCTION(BlueprintCallable, BlueprintGetter, Category="Level")
	bool IsFirstLevel() const;

	UFUNCTION(BlueprintCallable, BlueprintGetter, Category="Player")
	float GetPlayerPower() const;

	UFUNCTION(BlueprintCallable, BlueprintGetter, Category="Player")
	TSubclassOf<AWeapon> GetPlayerWeapon() const;
};
