#include "CombatCharacter.h"

#include "UpTime.h"
#include "Gameplay/AI/EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ACombatCharacter::ACombatCharacter()
	: bIsInvulnerable(false),
	bIsDying(false), HitSound(nullptr), HitEffect(nullptr), DeathSound(nullptr), DeathEffect(nullptr)
{}

float ACombatCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	const float RemainingDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	float Hitpoints = GetHitpoints();
	if (Hitpoints <= 0.f)
	{
		return 0.f;
	}

	if (RemainingDamage > 0.f)
	{
		Hitpoints -= RemainingDamage;
		SetHitpoints(Hitpoints);

		if (GetHitpoints() > 0.f)
		{
			SimulateHit(RemainingDamage, DamageEvent.DamageTypeClass, EventInstigator, DamageCauser);
		}
		else
		{
			Kill(EventInstigator, DamageCauser);
		}
	}

	return RemainingDamage;
}

void ACombatCharacter::SimulateHit_Implementation(float DamageTaken, TSubclassOf<UDamageType> DamageType,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (HitSound)
	{
		UGameplayStatics::SpawnSoundAttached(HitSound, GetRootComponent());
	}

	if (HitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetActorLocation());
	}
}

bool ACombatCharacter::Kill(AController* EventInstigator, AActor* DamageCauser)
{
	if (!CanDie(EventInstigator, DamageCauser))
	{
		return false;
	}

	bIsDying = true;

	const auto MyController = GetController();
	if (MyController)
	{
		MyController->StopMovement();
	}

	OnDeath.Broadcast();
	SimulateDeath(EventInstigator, DamageCauser);

	return true;
}

void ACombatCharacter::SimulateDeath_Implementation(AController* EventInstigator, AActor* DamageCauser)
{
	if (DeathSound)
	{
		UGameplayStatics::SpawnSoundAttached(DeathSound, GetRootComponent());
	}

	if (DeathEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathEffect, GetActorLocation());
	}
}

bool ACombatCharacter::IsInvulnerable() const
{
	return bIsInvulnerable;
}

void ACombatCharacter::SetInvulernable(bool IsInvulnerable)
{
	bIsInvulnerable = IsInvulnerable;
}


bool ACombatCharacter::CanDie(AController* EventInstigator, AActor* DamageCauser) const
{
	return !bIsDying && !IsPendingKill() && !IsInvulnerable();
}

bool ACombatCharacter::IsAlive() const
{
	return !IsDying();
}

bool ACombatCharacter::IsDying() const
{
	return bIsDying;
}
