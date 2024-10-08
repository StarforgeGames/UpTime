#include "Projectile.h"

#include "UpTime.h"
#include "Components/SphereComponent.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Items/Weapons/ProjectileWeapon.h"


AProjectile::AProjectile()
	: ProjectileLife(2.f),
	DamageType(UDamageType::StaticClass()),
	OwnerWeapon(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("Collision Component");
	SetRootComponent(CollisionComponent);
	CollisionComponent->InitSphereRadius(15.0f);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComponent->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Movement Component");
	MovementComponent->UpdatedComponent = CollisionComponent;
	MovementComponent->bAutoActivate = false;
	MovementComponent->bShouldBounce = false;
	MovementComponent->ProjectileGravityScale = 0.f;
	MovementComponent->InitialSpeed = 1000.f;

	ImpactEffectComponent = CreateDefaultSubobject<UParticleSystemComponent>("ImpactEffect");
	ImpactEffectComponent->bAutoActivate = false;
	ImpactEffectComponent->SetupAttachment(RootComponent);
}

void AProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MovementComponent->OnProjectileStop.AddDynamic(this, &AProjectile::OnImpact);
	const auto ProjectileOwner = GetInstigator();

	if (ProjectileOwner)
	{
		CollisionComponent->IgnoreActorWhenMoving(ProjectileOwner, true);
		ProjectileOwner->MoveIgnoreActorAdd(this);
	}

	OwnerWeapon = Cast<AProjectileWeapon>(GetOwner());
	if (!OwnerWeapon)
	{
		UE_LOG(LogUpTime, Error, TEXT("Could not retrieve OwnerWeapon"));
	}
	SetLifeSpan(ProjectileLife);
}

void AProjectile::Launch(float OwnerSpeed) const
{
	// Speed must not go negative, otherwise projectile while fly backwards
	const float Speed = FMath::Max(MovementComponent->InitialSpeed + OwnerSpeed, 1.f);
	const auto FiringVelocity = FVector::ForwardVector * Speed;
	MovementComponent->SetVelocityInLocalSpace(FiringVelocity);
	MovementComponent->Activate();
}

void AProjectile::OnImpact_Implementation(const FHitResult& HitResult)
{
	ImpactEffectComponent->SetWorldLocation(HitResult.ImpactPoint);
	ImpactEffectComponent->Activate();

	DealDamage(HitResult);
	OnSimulateImpact(HitResult);
	DisableAndDestroy();
}

void AProjectile::OnSimulateImpact_Implementation(const FHitResult& HitResult)
{}

void AProjectile::DisableAndDestroy()
{
	this->SetActorEnableCollision(false);

	MovementComponent->StopMovementImmediately();
	MeshComponent->SetVisibility(false);

	// give clients some time to show explosion
	SetLifeSpan(2.0f);
}
