#include "PowerSystemComponent.h"

#include "UpTime.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

UPowerSystemComponent::UPowerSystemComponent()
	: CurrentPower(10.0f),
	ConversionFactor(10)
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPowerSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	Activate();
}


void UPowerSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsActive())
	{
		CurrentPower -= DeltaTime;

		if (CurrentPower <= 0.0f)
		{
			if (!OnPowerDrained.ExecuteIfBound())
			{
				UE_LOG(LogUpTime, Error, TEXT("No object bound to drained event!"))
			}
			
			UE_LOG(LogUpTime, Log, TEXT("Power drained by time"))
			Deactivate();
		}

		const int PowercellDischarge = FMath::RoundToInt(CurrentPower) % ConversionFactor;
		
		if (PowercellDischarge == 0 && (!PowercelLDrainedAudio || !PowercelLDrainedAudio->IsPlaying()))
		{
			// TODO
			if (PowercellDrainedSound)
			{
				PowercelLDrainedAudio = UGameplayStatics::SpawnSoundAttached(PowercellDrainedSound, 
					GetOwner()->GetRootComponent());
			}
		}

		CurrentPower = FMath::Max(CurrentPower, 0.0f);
	}
}

void UPowerSystemComponent::ChargePower(const int ChargeValue)
{
	CurrentPower += ChargeValue;
}

void UPowerSystemComponent::DrainPower(float DrainValue)
{
	CurrentPower -= DrainValue;
}

void UPowerSystemComponent::SetCurrentPower(const float NewPower)
{
	CurrentPower = NewPower;
}
