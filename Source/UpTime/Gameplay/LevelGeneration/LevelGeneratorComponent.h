#pragma once

#include "CoreMinimal.h"
#include "TileSet.h"
#include "Components/ActorComponent.h"

#include "LevelGeneratorComponent.generated.h"


class ULevelStreamingDynamic;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UPTIME_API ULevelGeneratorComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	FString MapFolderPath;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Level Generation")
	int32 GridSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Level Generation")
	float TileSize;

	UPROPERTY()
	TArray<ULevelStreamingDynamic*> LevelTiles;

public:
	ULevelGeneratorComponent();

	UFUNCTION(BlueprintCallable, Category="Level Generation")
	void GenerateLevel(const FTileSet& CurrentTileSet);

protected:
	UFUNCTION(BlueprintCallable, Category="Level Generation")
	FString GetStartTileName(const FTileSet& CurrentTileSet) const;

	UFUNCTION(BlueprintCallable, Category="Level Generation")
	FString GetEndTileName(const FTileSet& CurrentTileSet) const;

	UFUNCTION(BlueprintCallable, Category="Level Generation")
	FString GetRandomTileName(const FTileSet& CurrentTileSet) const;

	UFUNCTION(BlueprintCallable, Category="Level Generation")
	FVector CalculateTilePosition(int Row, int Column) const;

public:
	UFUNCTION(BlueprintCallable, Category="Level Generation")
	void SpawnEnemies(int NumberOfEnemies, TSubclassOf<class AEnemyCharacter> EnemyType);

	UFUNCTION(BlueprintCallable, Category="Level Generation")
	void SpawnWeapons(int NumberOfWeapons, TArray<TSubclassOf<class APickup>> WeaponPickups);

protected:
	UFUNCTION(Category="Level Generator")
	TArray<ULevelStreamingDynamic*> GetTilesToPopulate() const;

public:
	UFUNCTION(BlueprintCallable, Category="Level Generation")
	bool AllTilesVisible();
};
