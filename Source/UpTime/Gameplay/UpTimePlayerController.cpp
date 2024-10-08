#include "UpTimePlayerController.h"

#include "UpTime.h"
#include "Actors/UpTimePlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AUpTimePlayerController::AUpTimePlayerController(): PlayerDeathMusic(nullptr), PlayerDeathMusicComponent(nullptr)
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AUpTimePlayerController::OnPossess(APawn* MyPawn)
{
	Super::OnPossess(MyPawn);

	const auto PlayerCharacter = Cast<AUpTimePlayerCharacter>(MyPawn);
	if (!PlayerCharacter)
	{
		UE_LOG(LogUpTime, Error, TEXT("Could not retrieve Player Character on BeginPlay"));
	}

	PlayerCharacter->OnDeath.AddDynamic(this, &AUpTimePlayerController::OnPlayerDeath);
}

void AUpTimePlayerController::OnPlayerDeath()
{
	if (!PlayerDeathMusic)
	{
		return;
	}

	PlayerDeathMusicComponent = UGameplayStatics::SpawnSound2D(GetWorld(), PlayerDeathMusic);
}