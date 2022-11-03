#include "LevelGeneratorComponent.h"


#include "NavigationSystem.h"
#include "UpTime.h"
#include "Actors/EnemyCharacter.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Gameplay/UpTimeGameInstance.h"
#include "Gameplay/Pickup.h"

ULevelGeneratorComponent::ULevelGeneratorComponent()
	: MapFolderPath(TEXT("/Game/UpTime/Maps")),
	GridSize(5),
	TileSize(1500.f)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void ULevelGeneratorComponent::GenerateLevel(const FTileSet& CurrentTileSet)
{
	const auto StartTileLocation = FVector2D(0, FMath::RandRange(0, GridSize - 1));
	const auto EndTileLocation = FVector2D(GridSize - 1, FMath::RandRange(0, GridSize - 1));
	
	for (int X = 0; X < GridSize; X++)
	{
		for (int Y = 0; Y < GridSize; Y++)
		{			
			FString TileName;
			if(X == StartTileLocation.X && Y == StartTileLocation.Y)
			{
				TileName = GetStartTileName(CurrentTileSet);
			}
			else if(X == EndTileLocation.X && Y == EndTileLocation.Y)
			{
				TileName = GetEndTileName(CurrentTileSet);
			}
			else
			{
				TileName = GetRandomTileName(CurrentTileSet);
			}
						
			const FVector Location = CalculateTilePosition(X, Y);
			const float Yaw = FMath::RandRange(0, 3) * 90.f;
			const FRotator Rotator = FRotator(0.f, Yaw, 0.f);

			bool Success;
			ULevelStreamingDynamic* Level = ULevelStreamingDynamic::LoadLevelInstance(GetWorld(), TileName, Location,
				Rotator, Success);
			LevelTiles.Add(Level);

			if(!Success)
			{
				UE_LOG(LogUpTime, Error, TEXT("Could not stream level %s"), *TileName)
				return;
			}
			
			Level->SetShouldBeLoaded(true);
			Level->SetShouldBeVisible(true);
		}
	}
}

void ULevelGeneratorComponent::SpawnEnemies(const int NumberOfEnemies, const TSubclassOf<AEnemyCharacter> EnemyType)
{
	if (!EnemyType)
	{		
		UE_LOG(LogUpTime, Error, TEXT("No enemy type set, cannot spawn enemies"));
		return;
	}
	
	UWorld* World = GetWorld();	
	auto NavSystem = UNavigationSystemV1::GetCurrent(World);

	if (!NavSystem)
	{
		UE_LOG(LogUpTime, Error, TEXT("Navigation System V1 has not been initialized, cannot spawn enemies"));
		return;
	}
	
	TArray<ULevelStreamingDynamic*> Tiles = GetTilesToPopulate();
	UE_LOG(LogUpTime, Log, TEXT("Spawning %i enemies on %i tiles"), NumberOfEnemies, Tiles.Num());

	for (int i = 0; i < NumberOfEnemies; i++)
	{
		const TArray<ULevelStreamingDynamic*>::ElementType Tile = Tiles[FMath::RandRange(0, Tiles.Num() - 1)];

		FActorSpawnParameters SpawnParams = FActorSpawnParameters();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FVector RandomLocation = NavSystem->GetRandomReachablePointInRadius(World, Tile->LevelTransform.GetLocation(), 500);
		FTransform Transform = FTransform(RandomLocation);

		AEnemyCharacter* Enemy = World->SpawnActor<AEnemyCharacter>(EnemyType.GetDefaultObject()->GetClass(), Transform, SpawnParams);
		if (Enemy)
		{
			Enemy->SpawnDefaultController();
			UE_LOG(LogUpTime, Log, TEXT("Spawned enemy %s on tile %s at location %s"), 
				*Enemy->GetName(), *Tile->PackageNameToLoad.ToString(), *Enemy->GetActorLocation().ToString())
		}
		else
		{
			UE_LOG(LogUpTime, Log, TEXT("Error when trying to spawn enemy at location %s"), *RandomLocation.ToString())
		}
	}
}

void ULevelGeneratorComponent::SpawnWeapons(int NumberOfWeapons, TArray<TSubclassOf<APickup>> WeaponPickups)
{
	if (WeaponPickups.Num() == 0)
	{		
		UE_LOG(LogUpTime, Error, TEXT("No weapon pickups set, cannot spawn weapons"));
		return;
	}
	
	UWorld* World = GetWorld();	
	auto NavSystem = UNavigationSystemV1::GetCurrent(World);

	if (!NavSystem)
	{
		UE_LOG(LogUpTime, Error, TEXT("Navigation System V1 has not been initialized, cannot spawn enemies"));
		return;
	}
	
	TArray<ULevelStreamingDynamic*> Tiles = GetTilesToPopulate();
	UE_LOG(LogUpTime, Log, TEXT("Spawning %i weapons on %i tiles"), NumberOfWeapons, Tiles.Num());

	for (int i = 0; i < NumberOfWeapons; i++)
	{
		const int32 RandomTile = FMath::RandRange(0, Tiles.Num() - 1);
		const TArray<ULevelStreamingDynamic*>::ElementType Tile = Tiles[RandomTile];

		FActorSpawnParameters SpawnParams = FActorSpawnParameters();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FVector RandomLocation = NavSystem->GetRandomReachablePointInRadius(World, Tile->LevelTransform.GetLocation(), 2000);
		RandomLocation.Z += 50;
		FTransform Transform = FTransform(RandomLocation);

		const int32 RandomWeapon = FMath::RandRange(0, WeaponPickups.Num() - 1);
		auto WeaponPickup = WeaponPickups[RandomWeapon];

		APickup* Pickup = World->SpawnActor<APickup>(WeaponPickup.GetDefaultObject()->GetClass(), Transform, SpawnParams);
		if (Pickup)
		{
			UE_LOG(LogUpTime, Log, TEXT("Spawned pickup %s on tile %s at location %s"), 
				*Pickup->GetName(), *Tile->PackageNameToLoad.ToString(), *Pickup->GetActorLocation().ToString())
		}
		else
		{
			UE_LOG(LogUpTime, Log, TEXT("Error when trying to spawn weapon at location %s"), *RandomLocation.ToString())
		}
	}
}

TArray<ULevelStreamingDynamic*> ULevelGeneratorComponent::GetTilesToPopulate() const
{
	TArray<ULevelStreamingDynamic*> Tiles = TArray<ULevelStreamingDynamic*>(LevelTiles);
	
	const int StartTileIndex = Tiles.IndexOfByPredicate([](ULevelStreamingDynamic *const Lvl)
	{
		return Lvl->PackageNameToLoad.ToString().Contains(TEXT("TileStart"));
	});	
	Tiles.RemoveAt(StartTileIndex);

	return Tiles;
}

bool ULevelGeneratorComponent::AllTilesVisible()
{
	bool bAllLoaded = true;

	for (int i = 0; i < LevelTiles.Num(); i++)
	{
		bAllLoaded &= LevelTiles[i]->IsLevelVisible();
	}
	
	return bAllLoaded;
}

FString ULevelGeneratorComponent::GetStartTileName(const FTileSet& CurrentTileSet) const
{
	TArray<FStringFormatArg> Args;
	Args.Add(FStringFormatArg(MapFolderPath));
	Args.Add(FStringFormatArg(CurrentTileSet.Name));
	Args.Add(FStringFormatArg(TEXT("Start")));

	return FString::Format(TEXT("{0}/{1}/{1}_Tile{2}"), Args);
}

FString ULevelGeneratorComponent::GetEndTileName(const FTileSet& CurrentTileSet) const
{
	TArray<FStringFormatArg> Args;
	Args.Add(FStringFormatArg(MapFolderPath));
	Args.Add(FStringFormatArg(CurrentTileSet.Name));
	Args.Add(FStringFormatArg(TEXT("End")));
	
	return FString::Format(TEXT("{0}/{1}/{1}_Tile{2}"), Args);
}

FString ULevelGeneratorComponent::GetRandomTileName(const FTileSet& CurrentTileSet) const
{
	TArray<FStringFormatArg> Args;
	Args.Add(FStringFormatArg(MapFolderPath));
	Args.Add(FStringFormatArg(CurrentTileSet.Name));
	
	const int TileNumber = FMath::RandRange(1, CurrentTileSet.Count);
	const FString PaddedNumber = FString::FromInt(TileNumber).LeftPad(2).Replace(TEXT(" "), TEXT("0"));
	Args.Add(FStringFormatArg(PaddedNumber));
	
	return FString::Format(TEXT("{0}/{1}/{1}_Tile{2}"), Args);
}

FVector ULevelGeneratorComponent::CalculateTilePosition(int32 Row, int32 Column) const
{
	const float X = Row * TileSize - (GridSize - 1) * TileSize / 2;
	const float Y = Column * TileSize - (GridSize - 1) * TileSize / 2;
	
	return FVector(X, Y, 0.f);
}
