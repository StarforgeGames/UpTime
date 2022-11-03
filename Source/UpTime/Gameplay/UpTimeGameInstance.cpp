#include "UpTimeGameInstance.h"

#include "UpTime.h"

UUpTimeGameInstance::UUpTimeGameInstance()
	: CurrentFloor(0),
	CurrentTileSetLevel(1),
	CurrentTileSetIndex(0),
	bIsFirstLevel(true),
	PlayerPower(0.0f)
{}

int UUpTimeGameInstance::GetCurrentFloor() const
{
	return CurrentFloor;
}

int UUpTimeGameInstance::GetCurrentTileSetLevel() const
{
	return CurrentTileSetLevel;
}

int UUpTimeGameInstance::GetCurrentTileSetIndex() const
{
	return CurrentTileSetIndex;
}

void UUpTimeGameInstance::NextLevel(float CurrentPlayerPower, TSubclassOf<class AWeapon> CurrentWeapon)
{
	CurrentFloor++;
	CurrentTileSetLevel++;
	
	UE_LOG(LogUpTime, Log, TEXT("Loading TileSetLevel %i, total floor %i"), CurrentFloor, CurrentTileSetLevel);
	
	PlayerPower = CurrentPlayerPower;
	PlayerWeapon = CurrentWeapon;
		
	bIsFirstLevel = false;
}

FTileSet UUpTimeGameInstance::GetCurrentTileSet() const
{
	return CurrentTileSet;
}

void UUpTimeGameInstance::SetCurrentTileSet(FTileSet NewTileSet)
{
	CurrentTileSet = NewTileSet;
}

void UUpTimeGameInstance::StartNewTileSet(const FTileSet NewTileSet)
{
	SetCurrentTileSet(NewTileSet);
	CurrentTileSetLevel = 1;
	CurrentTileSetIndex++;
}

bool UUpTimeGameInstance::IsCurrentTileSetFinished() const
{
	return CurrentTileSetLevel > CurrentTileSet.Levels;
}

bool UUpTimeGameInstance::IsFirstLevel() const
{
	return bIsFirstLevel;
}

float UUpTimeGameInstance::GetPlayerPower() const
{
	return PlayerPower;
}

TSubclassOf<class AWeapon> UUpTimeGameInstance::GetPlayerWeapon() const
{
	return PlayerWeapon;
}
