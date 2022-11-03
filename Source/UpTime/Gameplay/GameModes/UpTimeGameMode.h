#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Gameplay/LevelGeneration/TileSet.h"

#include "UpTimeGameMode.generated.h"

UCLASS(minimalapi)
class AUpTimeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUpTimeGameMode();

	void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable, Category="Game")
	void LoadNextLevel();
	
	UFUNCTION(BlueprintCallable, Category="Game")
	bool LoadNextTileSet();
	
	UFUNCTION(BlueprintPure, BlueprintCallable, Category="Game")
	int GetNumberOfEnemies() const;

protected:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Game")
	class UUpTimeGameInstance* GameInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level")
	class ULevelGeneratorComponent* LevelGenerator;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Level")
	FName MainLevelName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Level")
	TArray<FTileSet> TileSets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Level|Spawning")
	TArray<TSubclassOf<class APickup>> WeaponPickupTypes;

	/** The base number of enemies to spawn in a level. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Level|Spawning")
	int BaseEnemyCount;

	/** The number of enemies to add to a level based on the level number. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Level|Spawning")
	int ScalingEnemyCount;
	
};



