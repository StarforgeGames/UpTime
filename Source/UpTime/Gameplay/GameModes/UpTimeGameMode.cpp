#include "UpTimeGameMode.h"

#include "UpTime.h"
#include "Actors/UpTimePlayerCharacter.h"
#include "Gameplay/UpTimeGameInstance.h"
#include "Gameplay/LevelGeneration/LevelGeneratorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Weapons/Weapon.h"

AUpTimeGameMode::AUpTimeGameMode()
	: MainLevelName(TEXT("/Game/UpTime/Maps/Arena")),
	BaseEnemyCount(15),
	ScalingEnemyCount(2)
{
	LevelGenerator = CreateDefaultSubobject<ULevelGeneratorComponent>("Level Generator");
}

void AUpTimeGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GameInstance = Cast<UUpTimeGameInstance>(GetGameInstance());
	if (!GameInstance)
	{
		UE_LOG(LogUpTime, Error, TEXT("Could not retrieve Game Instance, cannot generate levels"));
		return;
	}

	if (TileSets.Num() < 1)
	{
		UE_LOG(LogUpTime, Error, TEXT("TileSets not set, cannot generate levels"));
		return;
	}

	// Only initialize on new game
	const FTileSet InstanceTileSet = GameInstance->GetCurrentTileSet();
	if (InstanceTileSet.Name.Len() == 0)
	{
		GameInstance->SetCurrentTileSet(TileSets[GameInstance->GetCurrentTileSetIndex()]);
	}
}

void AUpTimeGameMode::LoadNextLevel()
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogUpTime, Warning, TEXT("World is NULL, cannot load next level in GameMode::LoadNextLevel"))
		return;
	}

	AActor* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0);
	AUpTimePlayerCharacter* PlayerCharacter = Cast<AUpTimePlayerCharacter>(PlayerPawn);
	if (!PlayerCharacter)
	{
		UE_LOG(LogUpTime, Error, TEXT("Could not get player character in GameMode::LoadNextLevel"));
		return;
	}

	const AWeapon* Weapon = IArmedActor::Execute_GetWeapon(PlayerCharacter);
	UClass* WeaponClass = nullptr;
	if (Weapon)
	{
		WeaponClass = Weapon->GetClass();
	}

	GameInstance->NextLevel(PlayerCharacter->GetCurrentPower(), WeaponClass);

	if (GameInstance->IsCurrentTileSetFinished())
	{
		UE_LOG(LogUpTime, Log, TEXT("Current Tileset %i is finished, loading next"),
			GameInstance->GetCurrentTileSetIndex());
		if (!LoadNextTileSet())
		{
			UE_LOG(LogUpTime, Log, TEXT("No next tileset, loading final level"));
			UGameplayStatics::OpenLevel(World, TEXT("/Game/UpTime/Maps/FinalLevel"));
			return;
		}
	}

	UGameplayStatics::OpenLevel(World, MainLevelName);
}

bool AUpTimeGameMode::LoadNextTileSet()
{
	if (GameInstance->GetCurrentTileSetIndex() + 1 >= TileSets.Num())
	{
		return false;
	}

	const FTileSet NewTileSet = TileSets[GameInstance->GetCurrentTileSetIndex() + 1];
	GameInstance->StartNewTileSet(NewTileSet);
	UE_LOG(LogUpTime, Log, TEXT("Started new Tileset %i of %i"), GameInstance->GetCurrentTileSetIndex(),
		TileSets.Num());

	return true;
}

int AUpTimeGameMode::GetNumberOfEnemies() const
{
	if (!GameInstance)
	{
		UE_LOG(LogUpTime, Error, TEXT("GameInstance not initialized in GetNumberOfEnemies"));
		return BaseEnemyCount;
	}

	return BaseEnemyCount + GameInstance->GetCurrentTileSetLevel() * 2;
}
