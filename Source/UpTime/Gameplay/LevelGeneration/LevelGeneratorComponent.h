#pragma once

#include "CoreMinimal.h"
#include "TileSet.h"
#include "Components/ActorComponent.h"

#include "LevelGeneratorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPTIME_API ULevelGeneratorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULevelGeneratorComponent();

	UFUNCTION(BlueprintCallable, Category="Level Generation")
	void GenerateLevel(const FTileSet& CurrentTileSet);

	UFUNCTION(BlueprintCallable, Category="Level Generation")
	void SpawnEnemies(int NumberOfEnemies, TSubclassOf<class AEnemyCharacter> EnemyType);

	UFUNCTION(BlueprintCallable, Category="Level Generation")
	void SpawnWeapons(int NumberOfWeapons, TArray<TSubclassOf<class APickup>> WeaponPickups);

	UFUNCTION(BlueprintCallable, Category="Level Generation")
	bool AllTilesVisible();	

protected:
	UFUNCTION(Category="Level Generator")
	TArray<class ULevelStreamingDynamic*> GetTilesToPopulate() const;
	
	UFUNCTION(BlueprintCallable, Category="Level Generation")
	FString GetStartTileName(const FTileSet& CurrentTileSet) const;
	
	UFUNCTION(BlueprintCallable, Category="Level Generation")
	FString GetEndTileName(const FTileSet& CurrentTileSet) const;
	
	UFUNCTION(BlueprintCallable, Category="Level Generation")
	FString GetRandomTileName(const FTileSet& CurrentTileSet) const;
	
	UFUNCTION(BlueprintCallable, Category="Level Generation")
	FVector CalculateTilePosition(int32 Row, int32 Column) const;

	UPROPERTY()
	FString MapFolderPath;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Level Generation")
	int32 GridSize;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Level Generation")
	float TileSize;

	UPROPERTY()
	TArray<class ULevelStreamingDynamic*> LevelTiles;
};
