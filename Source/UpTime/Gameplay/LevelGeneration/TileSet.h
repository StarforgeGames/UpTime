#pragma once

#include "CoreMinimal.h"

#include "TileSet.generated.h"

USTRUCT(BlueprintType)
struct FTileSet
{
	GENERATED_USTRUCT_BODY()

	FTileSet()
		: Count(0),
		Levels(0)
	{}


	FTileSet(const FString& Name, const int32 Count, const int32 Levels, TSubclassOf<class AEnemyCharacter> EnemyType)
		: Name(Name),
		Count(Count),
		Levels(Levels),
		EnemyType(EnemyType)
	{}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TileSet")
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TileSet")
	int32 Count;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TileSet")
	int32 Levels;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TileSet")
	TSubclassOf<class AEnemyCharacter> EnemyType;
};
