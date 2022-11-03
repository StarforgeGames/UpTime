#include "Weapon.h"

#include "UpTime.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Actors/Common/ArmedActor.h"

// Sets default values
AWeapon::AWeapon()
	: bIsLoopedFireSound(false),
	FiringSound(nullptr),
	FiringLoopSound(nullptr),
	FireFinishSound(nullptr),
	FiringEffect(nullptr),
	FiringAudioComponent(nullptr),
	MuzzleConfiguration(EMuzzleConfiguration::Single),
	bWantsToFire(false),
	LastTimeFiring(0.f),
	CurrentState(EWeaponState::Idle)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AWeapon::Destroyed()
{
	Super::Destroyed();
	StopSimulatingWeaponFire();
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::OnEquipped(TScriptInterface<IArmedActor> NewOwner)
{
	AActor* Actor = Cast<AActor>(NewOwner.GetObject());

	if (!ensureMsgf(Actor, TEXT("Weapon owner must be an Actor")))
	{
		return;
	}

	SetOwner(Actor);
}

void AWeapon::StartFiring()
{
	bWantsToFire = true;

	if (!CanFire())
	{
		UE_LOG(LogUpTime, Warning, TEXT("Can't fire"));
		return;
	}

	if (CurrentState != EWeaponState::Firing)
	{
		const float GameTime = GetWorld()->GetTimeSeconds();
		float NextFireTime = LastTimeFiring + TimeBetweenShots - GameTime;
		NextFireTime = FMath::Max(NextFireTime, 0.f);

		GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &AWeapon::HandleFiring,
			TimeBetweenShots, true, NextFireTime);

		SetCurrentState(EWeaponState::Firing);
	}
}

void AWeapon::StopFiring()
{
	bWantsToFire = false;

	if (CurrentState == EWeaponState::Firing)
	{
		StopSimulatingWeaponFire();
		GetWorldTimerManager().ClearTimer(TimerHandle_HandleFiring);
		SetCurrentState(EWeaponState::Idle);
	}
}

bool AWeapon::CanFire() const
{
	return CurrentState == EWeaponState::Idle || CurrentState == EWeaponState::Firing;
}

EWeaponState AWeapon::GetCurrentState() const
{
	return CurrentState;
}

void AWeapon::SetCurrentState(const EWeaponState NewState)
{
	CurrentState = NewState;
}

void AWeapon::HandleFiring()
{
	AActor* WeaponOwner = GetOwner();
	
	if (!CanFire() || !IArmedActor::Execute_CanFire(WeaponOwner))
	{
		StopFiring();
	}

	FireShot();
	SimulateWeaponFire();

	LastTimeFiring = GetWorld()->GetTimeSeconds();
}

void AWeapon::SimulateWeaponFire()
{
	if (bIsLoopedFireSound)
	{
		if (!FiringAudioComponent)
		{
			FiringAudioComponent = PlaySound(FiringLoopSound);
		}
	}
	else
	{
		PlaySound(FiringSound);
	}
		
	AActor* WeaponOwner = GetOwner();
	IArmedActor::Execute_OnFiringShot(WeaponOwner);

	if (!FiringEffect)
	{
		return;
	}

	IArmedActor* ArmedActor = Cast<IArmedActor>(WeaponOwner);
	if (!ensureMsgf(ArmedActor, TEXT("Weapon must have an owner to be fired")))
	{
		return;
	}

	TArray<FVector> MuzzleLocations = IArmedActor::Execute_GetWeaponMuzzleLocations(WeaponOwner, MuzzleConfiguration);
	
	for (const FVector Location : MuzzleLocations)
	{
		UGameplayStatics::SpawnEmitterAttached(
			FiringEffect,
			WeaponOwner->GetRootComponent(),
			NAME_None,
			Location,
			FRotator::ZeroRotator,
			FVector(1),
			EAttachLocation::KeepWorldPosition);
	}
}

UAudioComponent* AWeapon::PlaySound(USoundCue* Sound) const
{
	UAudioComponent* AudioComponent = nullptr;
	AActor* WeaponOwner = GetOwner();

	if (Sound && WeaponOwner)
	{
		AudioComponent = UGameplayStatics::SpawnSoundAttached(Sound, WeaponOwner->GetRootComponent());
	}

	return AudioComponent;
}

void AWeapon::StopSimulatingWeaponFire()
{
	if (!FiringAudioComponent)
	{
		return;
	}

	FiringAudioComponent->FadeOut(0.1f, 0.0f);
	FiringAudioComponent = nullptr;

	PlaySound(FireFinishSound);
}

FVector AWeapon::GetAimDirection() const
{
	AActor* WeaponOwner = GetOwner();
	if (!ensureMsgf(WeaponOwner, TEXT("Weapon owner is null, cannot get aim direction")))
	{
		return FVector();
	}

	return WeaponOwner->GetActorForwardVector();
}
