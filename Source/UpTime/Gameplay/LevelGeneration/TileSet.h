#pragma once

#include "CoreMinimal.h"

#include "TileSet.generated.h"

USTRUCT(BlueprintType)
struct FTileSet
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TileSet")
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TileSet")
	int Count;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TileSet")
	int Levels;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TileSet")
	TSubclassOf<class AEnemyCharacter> EnemyType;

	FTileSet()
		: Count(0),
		Levels(0)
	{}

	FTileSet(const FString& Name, const int Count, const int Levels, TSubclassOf<class AEnemyCharacter> EnemyType)
		: Name(Name),
		Count(Count),
		Levels(Levels),
		EnemyType(EnemyType)
	{}
};
